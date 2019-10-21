/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2007 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#include "config.h"

#include "backends/jack.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "alMain.h"
#include "alu.h"
#include "alconfig.h"
#include "ringbuffer.h"
#include "threads.h"
#include "compat.h"

#include <jack/jack.h>
#include <jack/ringbuffer.h>


static const ALCchar jackDevice[] = "JACK Default";


#ifdef HAVE_DYNLOAD
#define JACK_FUNCS(MAGIC)          \
    MAGIC(jack_client_open);       \
    MAGIC(jack_client_close);      \
    MAGIC(jack_client_name_size);  \
    MAGIC(jack_get_client_name);   \
    MAGIC(jack_connect);           \
    MAGIC(jack_activate);          \
    MAGIC(jack_deactivate);        \
    MAGIC(jack_port_register);     \
    MAGIC(jack_port_unregister);   \
    MAGIC(jack_port_get_buffer);   \
    MAGIC(jack_port_name);         \
    MAGIC(jack_get_ports);         \
    MAGIC(jack_free);              \
    MAGIC(jack_get_sample_rate);   \
    MAGIC(jack_set_error_function); \
    MAGIC(jack_set_process_callback); \
    MAGIC(jack_set_buffer_size_callback); \
    MAGIC(jack_set_buffer_size);   \
    MAGIC(jack_get_buffer_size);

static void *jack_handle;
#define MAKE_FUNC(f) static decltype(f) * p##f
JACK_FUNCS(MAKE_FUNC);
static decltype(jack_error_callback) * pjack_error_callback;
#undef MAKE_FUNC

#ifndef IN_IDE_PARSER
#define jack_client_open pjack_client_open
#define jack_client_close pjack_client_close
#define jack_client_name_size pjack_client_name_size
#define jack_get_client_name pjack_get_client_name
#define jack_connect pjack_connect
#define jack_activate pjack_activate
#define jack_deactivate pjack_deactivate
#define jack_port_register pjack_port_register
#define jack_port_unregister pjack_port_unregister
#define jack_port_get_buffer pjack_port_get_buffer
#define jack_port_name pjack_port_name
#define jack_get_ports pjack_get_ports
#define jack_free pjack_free
#define jack_get_sample_rate pjack_get_sample_rate
#define jack_set_error_function pjack_set_error_function
#define jack_set_process_callback pjack_set_process_callback
#define jack_set_buffer_size_callback pjack_set_buffer_size_callback
#define jack_set_buffer_size pjack_set_buffer_size
#define jack_get_buffer_size pjack_get_buffer_size
#define jack_error_callback (*pjack_error_callback)
#endif
#endif


static jack_options_t ClientOptions = JackNullOption;

static ALCboolean jack_load(void)
{
    ALCboolean error = ALC_FALSE;

#ifdef HAVE_DYNLOAD
    if(!jack_handle)
    {
        std::string missing_funcs;

#ifdef _WIN32
#define JACKLIB "libjack.dll"
#else
#define JACKLIB "libjack.so.0"
#endif
        jack_handle = LoadLib(JACKLIB);
        if(!jack_handle)
        {
            WARN("Failed to load %s\n", JACKLIB);
            return ALC_FALSE;
        }

        error = ALC_FALSE;
#define LOAD_FUNC(f) do {                                                     \
    p##f = reinterpret_cast<decltype(p##f)>(GetSymbol(jack_handle, #f));      \
    if(p##f == nullptr) {                                                     \
        error = ALC_TRUE;                                                     \
        missing_funcs += "\n" #f;                                             \
    }                                                                         \
} while(0)
        JACK_FUNCS(LOAD_FUNC);
#undef LOAD_FUNC
        /* Optional symbols. These don't exist in all versions of JACK. */
#define LOAD_SYM(f) p##f = reinterpret_cast<decltype(p##f)>(GetSymbol(jack_handle, #f))
        LOAD_SYM(jack_error_callback);
#undef LOAD_SYM

        if(error)
        {
            WARN("Missing expected functions:%s\n", missing_funcs.c_str());
            CloseLib(jack_handle);
            jack_handle = NULL;
        }
    }
#endif

    return !error;
}


struct ALCjackPlayback final : public ALCbackend {
    jack_client_t *Client;
    jack_port_t *Port[MAX_OUTPUT_CHANNELS];

    ll_ringbuffer_t *Ring;
    alsem_t Sem;

    ATOMIC(ALenum) killNow;
    althrd_t thread;
};

static int ALCjackPlayback_bufferSizeNotify(jack_nframes_t numframes, void *arg);

static int ALCjackPlayback_process(jack_nframes_t numframes, void *arg);
static int ALCjackPlayback_mixerProc(void *arg);

static void ALCjackPlayback_Construct(ALCjackPlayback *self, ALCdevice *device);
static void ALCjackPlayback_Destruct(ALCjackPlayback *self);
static ALCenum ALCjackPlayback_open(ALCjackPlayback *self, const ALCchar *name);
static ALCboolean ALCjackPlayback_reset(ALCjackPlayback *self);
static ALCboolean ALCjackPlayback_start(ALCjackPlayback *self);
static void ALCjackPlayback_stop(ALCjackPlayback *self);
static DECLARE_FORWARD2(ALCjackPlayback, ALCbackend, ALCenum, captureSamples, void*, ALCuint)
static DECLARE_FORWARD(ALCjackPlayback, ALCbackend, ALCuint, availableSamples)
static ClockLatency ALCjackPlayback_getClockLatency(ALCjackPlayback *self);
static DECLARE_FORWARD(ALCjackPlayback, ALCbackend, void, lock)
static DECLARE_FORWARD(ALCjackPlayback, ALCbackend, void, unlock)
DECLARE_DEFAULT_ALLOCATORS(ALCjackPlayback)

DEFINE_ALCBACKEND_VTABLE(ALCjackPlayback);


static void ALCjackPlayback_Construct(ALCjackPlayback *self, ALCdevice *device)
{
    new (self) ALCjackPlayback{};
    ALCbackend_Construct(STATIC_CAST(ALCbackend, self), device);
    SET_VTABLE2(ALCjackPlayback, ALCbackend, self);

    alsem_init(&self->Sem, 0);

    self->Client = NULL;
    for(ALsizei i{0};i < MAX_OUTPUT_CHANNELS;i++)
        self->Port[i] = NULL;
    self->Ring = NULL;

    ATOMIC_INIT(&self->killNow, AL_TRUE);
}

static void ALCjackPlayback_Destruct(ALCjackPlayback *self)
{
    if(self->Client)
    {
        for(ALsizei i{0};i < MAX_OUTPUT_CHANNELS;i++)
        {
            if(self->Port[i])
                jack_port_unregister(self->Client, self->Port[i]);
            self->Port[i] = NULL;
        }
        jack_client_close(self->Client);
        self->Client = NULL;
    }

    alsem_destroy(&self->Sem);

    ALCbackend_Destruct(STATIC_CAST(ALCbackend, self));
    self->~ALCjackPlayback();
}


static int ALCjackPlayback_bufferSizeNotify(jack_nframes_t numframes, void *arg)
{
    ALCjackPlayback *self = static_cast<ALCjackPlayback*>(arg);
    ALCdevice *device = STATIC_CAST(ALCbackend,self)->mDevice;
    ALuint bufsize;

    ALCjackPlayback_lock(self);
    device->UpdateSize = numframes;
    device->NumUpdates = 2;

    bufsize = device->UpdateSize;
    if(ConfigValueUInt(device->DeviceName.c_str(), "jack", "buffer-size", &bufsize))
        bufsize = maxu(NextPowerOf2(bufsize), device->UpdateSize);
    device->NumUpdates = (bufsize+device->UpdateSize) / device->UpdateSize;

    TRACE("%u update size x%u\n", device->UpdateSize, device->NumUpdates);

    ll_ringbuffer_free(self->Ring);
    self->Ring = ll_ringbuffer_create(bufsize,
        FrameSizeFromDevFmt(device->FmtChans, device->FmtType, device->mAmbiOrder),
        true
    );
    if(!self->Ring)
    {
        ERR("Failed to reallocate ringbuffer\n");
        aluHandleDisconnect(device, "Failed to reallocate %u-sample buffer", bufsize);
    }
    ALCjackPlayback_unlock(self);
    return 0;
}


static int ALCjackPlayback_process(jack_nframes_t numframes, void *arg)
{
    ALCjackPlayback *self = static_cast<ALCjackPlayback*>(arg);
    jack_default_audio_sample_t *out[MAX_OUTPUT_CHANNELS];
    jack_nframes_t total{0};
    jack_nframes_t todo;
    ALsizei i, c, numchans;

    auto data = ll_ringbuffer_get_read_vector(self->Ring);

    for(c = 0;c < MAX_OUTPUT_CHANNELS && self->Port[c];c++)
        out[c] = static_cast<float*>(jack_port_get_buffer(self->Port[c], numframes));
    numchans = c;

    todo = minu(numframes, data.first.len);
    for(c = 0;c < numchans;c++)
    {
        const ALfloat *RESTRICT in = ((ALfloat*)data.first.buf) + c;
        for(i = 0;(jack_nframes_t)i < todo;i++)
            out[c][i] = in[i*numchans];
        out[c] += todo;
    }
    total += todo;

    todo = minu(numframes-total, data.second.len);
    if(todo > 0)
    {
        for(c = 0;c < numchans;c++)
        {
            const ALfloat *RESTRICT in = ((ALfloat*)data.second.buf) + c;
            for(i = 0;(jack_nframes_t)i < todo;i++)
                out[c][i] = in[i*numchans];
            out[c] += todo;
        }
        total += todo;
    }

    ll_ringbuffer_read_advance(self->Ring, total);
    alsem_post(&self->Sem);

    if(numframes > total)
    {
        todo = numframes-total;
        for(c = 0;c < numchans;c++)
        {
            for(i = 0;(jack_nframes_t)i < todo;i++)
                out[c][i] = 0.0f;
        }
    }

    return 0;
}

static int ALCjackPlayback_mixerProc(void *arg)
{
    ALCjackPlayback *self = static_cast<ALCjackPlayback*>(arg);
    ALCdevice *device = STATIC_CAST(ALCbackend,self)->mDevice;

    SetRTPriority();
    althrd_setname(MIXER_THREAD_NAME);

    ALCjackPlayback_lock(self);
    while(!ATOMIC_LOAD(&self->killNow, almemory_order_acquire) &&
          ATOMIC_LOAD(&device->Connected, almemory_order_acquire))
    {
        ALuint todo, len1, len2;

        if(ll_ringbuffer_write_space(self->Ring) < device->UpdateSize)
        {
            ALCjackPlayback_unlock(self);
            alsem_wait(&self->Sem);
            ALCjackPlayback_lock(self);
            continue;
        }

        auto data = ll_ringbuffer_get_write_vector(self->Ring);
        todo  = data.first.len + data.second.len;
        todo -= todo%device->UpdateSize;

        len1 = minu(data.first.len, todo);
        len2 = minu(data.second.len, todo-len1);

        aluMixData(device, data.first.buf, len1);
        if(len2 > 0)
            aluMixData(device, data.second.buf, len2);
        ll_ringbuffer_write_advance(self->Ring, todo);
    }
    ALCjackPlayback_unlock(self);

    return 0;
}


static ALCenum ALCjackPlayback_open(ALCjackPlayback *self, const ALCchar *name)
{
    ALCdevice *device = STATIC_CAST(ALCbackend, self)->mDevice;
    const char *client_name = "alsoft";
    jack_status_t status;

    if(!name)
        name = jackDevice;
    else if(strcmp(name, jackDevice) != 0)
        return ALC_INVALID_VALUE;

    self->Client = jack_client_open(client_name, ClientOptions, &status, NULL);
    if(self->Client == NULL)
    {
        ERR("jack_client_open() failed, status = 0x%02x\n", status);
        return ALC_INVALID_VALUE;
    }
    if((status&JackServerStarted))
        TRACE("JACK server started\n");
    if((status&JackNameNotUnique))
    {
        client_name = jack_get_client_name(self->Client);
        TRACE("Client name not unique, got `%s' instead\n", client_name);
    }

    jack_set_process_callback(self->Client, ALCjackPlayback_process, self);
    jack_set_buffer_size_callback(self->Client, ALCjackPlayback_bufferSizeNotify, self);

    device->DeviceName = name;
    return ALC_NO_ERROR;
}

static ALCboolean ALCjackPlayback_reset(ALCjackPlayback *self)
{
    ALCdevice *device = STATIC_CAST(ALCbackend, self)->mDevice;
    ALsizei numchans, i;
    ALuint bufsize;

    for(i = 0;i < MAX_OUTPUT_CHANNELS;i++)
    {
        if(self->Port[i])
            jack_port_unregister(self->Client, self->Port[i]);
        self->Port[i] = NULL;
    }

    /* Ignore the requested buffer metrics and just keep one JACK-sized buffer
     * ready for when requested.
     */
    device->Frequency = jack_get_sample_rate(self->Client);
    device->UpdateSize = jack_get_buffer_size(self->Client);
    device->NumUpdates = 2;

    bufsize = device->UpdateSize;
    if(ConfigValueUInt(device->DeviceName.c_str(), "jack", "buffer-size", &bufsize))
        bufsize = maxu(NextPowerOf2(bufsize), device->UpdateSize);
    device->NumUpdates = (bufsize+device->UpdateSize) / device->UpdateSize;

    /* Force 32-bit float output. */
    device->FmtType = DevFmtFloat;

    numchans = ChannelsFromDevFmt(device->FmtChans, device->mAmbiOrder);
    for(i = 0;i < numchans;i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "channel_%d", i+1);
        self->Port[i] = jack_port_register(self->Client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        if(self->Port[i] == NULL)
        {
            ERR("Not enough JACK ports available for %s output\n", DevFmtChannelsString(device->FmtChans));
            if(i == 0) return ALC_FALSE;
            break;
        }
    }
    if(i < numchans)
    {
        if(i == 1)
            device->FmtChans = DevFmtMono;
        else
        {
            for(--i;i >= 2;i--)
            {
                jack_port_unregister(self->Client, self->Port[i]);
                self->Port[i] = NULL;
            }
            device->FmtChans = DevFmtStereo;
        }
    }

    ll_ringbuffer_free(self->Ring);
    self->Ring = ll_ringbuffer_create(bufsize,
        FrameSizeFromDevFmt(device->FmtChans, device->FmtType, device->mAmbiOrder),
        true
    );
    if(!self->Ring)
    {
        ERR("Failed to allocate ringbuffer\n");
        return ALC_FALSE;
    }

    SetDefaultChannelOrder(device);

    return ALC_TRUE;
}

static ALCboolean ALCjackPlayback_start(ALCjackPlayback *self)
{
    const char **ports;
    ALsizei i;

    if(jack_activate(self->Client))
    {
        ERR("Failed to activate client\n");
        return ALC_FALSE;
    }

    ports = jack_get_ports(self->Client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if(ports == NULL)
    {
        ERR("No physical playback ports found\n");
        jack_deactivate(self->Client);
        return ALC_FALSE;
    }
    for(i = 0;i < MAX_OUTPUT_CHANNELS && self->Port[i];i++)
    {
        if(!ports[i])
        {
            ERR("No physical playback port for \"%s\"\n", jack_port_name(self->Port[i]));
            break;
        }
        if(jack_connect(self->Client, jack_port_name(self->Port[i]), ports[i]))
            ERR("Failed to connect output port \"%s\" to \"%s\"\n", jack_port_name(self->Port[i]), ports[i]);
    }
    jack_free(ports);

    ATOMIC_STORE(&self->killNow, AL_FALSE, almemory_order_release);
    if(althrd_create(&self->thread, ALCjackPlayback_mixerProc, self) != althrd_success)
    {
        jack_deactivate(self->Client);
        return ALC_FALSE;
    }

    return ALC_TRUE;
}

static void ALCjackPlayback_stop(ALCjackPlayback *self)
{
    int res;

    if(self->killNow.exchange(AL_TRUE, std::memory_order_acq_rel))
        return;

    alsem_post(&self->Sem);
    althrd_join(self->thread, &res);

    jack_deactivate(self->Client);
}


static ClockLatency ALCjackPlayback_getClockLatency(ALCjackPlayback *self)
{
    ALCdevice *device = STATIC_CAST(ALCbackend, self)->mDevice;
    ClockLatency ret;

    ALCjackPlayback_lock(self);
    ret.ClockTime = GetDeviceClockTime(device);
    ret.Latency  = std::chrono::seconds{ll_ringbuffer_read_space(self->Ring)};
    ret.Latency /= device->Frequency;
    ALCjackPlayback_unlock(self);

    return ret;
}


static void jack_msg_handler(const char *message)
{
    WARN("%s\n", message);
}

bool JackBackendFactory::init()
{
    void (*old_error_cb)(const char*);
    jack_client_t *client;
    jack_status_t status;

    if(!jack_load())
        return false;

    if(!GetConfigValueBool(NULL, "jack", "spawn-server", 0))
        ClientOptions = static_cast<jack_options_t>(ClientOptions | JackNoStartServer);

    old_error_cb = (&jack_error_callback ? jack_error_callback : NULL);
    jack_set_error_function(jack_msg_handler);
    client = jack_client_open("alsoft", ClientOptions, &status, NULL);
    jack_set_error_function(old_error_cb);
    if(!client)
    {
        WARN("jack_client_open() failed, 0x%02x\n", status);
        if((status&JackServerFailed) && !(ClientOptions&JackNoStartServer))
            ERR("Unable to connect to JACK server\n");
        return false;
    }

    jack_client_close(client);
    return true;
}

void JackBackendFactory::deinit()
{
#ifdef HAVE_DYNLOAD
    if(jack_handle)
        CloseLib(jack_handle);
    jack_handle = nullptr;
#endif
}

bool JackBackendFactory::querySupport(ALCbackend_Type type)
{ return (type == ALCbackend_Playback); }

void JackBackendFactory::probe(enum DevProbe type, std::string *outnames)
{
    switch(type)
    {
        case ALL_DEVICE_PROBE:
            /* Includes null char. */
            outnames->append(jackDevice, sizeof(jackDevice));
            break;

        case CAPTURE_DEVICE_PROBE:
            break;
    }
}

ALCbackend *JackBackendFactory::createBackend(ALCdevice *device, ALCbackend_Type type)
{
    if(type == ALCbackend_Playback)
    {
        ALCjackPlayback *backend;
        NEW_OBJ(backend, ALCjackPlayback)(device);
        if(!backend) return nullptr;
        return STATIC_CAST(ALCbackend, backend);
    }

    return nullptr;
}

BackendFactory &JackBackendFactory::getFactory()
{
    static JackBackendFactory factory{};
    return factory;
}
