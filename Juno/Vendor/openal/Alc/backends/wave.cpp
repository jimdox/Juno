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

#include "backends/wave.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>

#include <chrono>
#include <thread>
#include <vector>

#include "alMain.h"
#include "alu.h"
#include "alconfig.h"
#include "compat.h"


namespace {

using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;

constexpr ALCchar waveDevice[] = "Wave File Writer";

constexpr ALubyte SUBTYPE_PCM[]{
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa,
    0x00, 0x38, 0x9b, 0x71
};
constexpr ALubyte SUBTYPE_FLOAT[]{
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa,
    0x00, 0x38, 0x9b, 0x71
};

constexpr ALubyte SUBTYPE_BFORMAT_PCM[]{
    0x01, 0x00, 0x00, 0x00, 0x21, 0x07, 0xd3, 0x11, 0x86, 0x44, 0xc8, 0xc1,
    0xca, 0x00, 0x00, 0x00
};

constexpr ALubyte SUBTYPE_BFORMAT_FLOAT[]{
    0x03, 0x00, 0x00, 0x00, 0x21, 0x07, 0xd3, 0x11, 0x86, 0x44, 0xc8, 0xc1,
    0xca, 0x00, 0x00, 0x00
};

void fwrite16le(ALushort val, FILE *f)
{
    ALubyte data[2]{ static_cast<ALubyte>(val&0xff), static_cast<ALubyte>((val>>8)&0xff) };
    fwrite(data, 1, 2, f);
}

void fwrite32le(ALuint val, FILE *f)
{
    ALubyte data[4]{ static_cast<ALubyte>(val&0xff), static_cast<ALubyte>((val>>8)&0xff),
        static_cast<ALubyte>((val>>16)&0xff), static_cast<ALubyte>((val>>24)&0xff) };
    fwrite(data, 1, 4, f);
}


struct ALCwaveBackend final : public ALCbackend {
    FILE *mFile;
    long mDataStart;

    std::vector<ALbyte> mBuffer;

    ATOMIC(ALenum) killNow;
    std::thread thread;
};

int ALCwaveBackend_mixerProc(ALCwaveBackend *self);

void ALCwaveBackend_Construct(ALCwaveBackend *self, ALCdevice *device);
void ALCwaveBackend_Destruct(ALCwaveBackend *self);
ALCenum ALCwaveBackend_open(ALCwaveBackend *self, const ALCchar *name);
ALCboolean ALCwaveBackend_reset(ALCwaveBackend *self);
ALCboolean ALCwaveBackend_start(ALCwaveBackend *self);
void ALCwaveBackend_stop(ALCwaveBackend *self);
DECLARE_FORWARD2(ALCwaveBackend, ALCbackend, ALCenum, captureSamples, void*, ALCuint)
DECLARE_FORWARD(ALCwaveBackend, ALCbackend, ALCuint, availableSamples)
DECLARE_FORWARD(ALCwaveBackend, ALCbackend, ClockLatency, getClockLatency)
DECLARE_FORWARD(ALCwaveBackend, ALCbackend, void, lock)
DECLARE_FORWARD(ALCwaveBackend, ALCbackend, void, unlock)
DECLARE_DEFAULT_ALLOCATORS(ALCwaveBackend)

DEFINE_ALCBACKEND_VTABLE(ALCwaveBackend);


void ALCwaveBackend_Construct(ALCwaveBackend *self, ALCdevice *device)
{
    new (self) ALCwaveBackend{};
    ALCbackend_Construct(STATIC_CAST(ALCbackend, self), device);
    SET_VTABLE2(ALCwaveBackend, ALCbackend, self);

    self->mFile = nullptr;
    self->mDataStart = -1;

    ATOMIC_INIT(&self->killNow, AL_TRUE);
}

void ALCwaveBackend_Destruct(ALCwaveBackend *self)
{
    if(self->mFile)
        fclose(self->mFile);
    self->mFile = nullptr;

    ALCbackend_Destruct(STATIC_CAST(ALCbackend, self));
    self->~ALCwaveBackend();
}

int ALCwaveBackend_mixerProc(ALCwaveBackend *self)
{
    ALCdevice *device = STATIC_CAST(ALCbackend, self)->mDevice;
    const milliseconds restTime{device->UpdateSize*1000/device->Frequency / 2};

    althrd_setname(MIXER_THREAD_NAME);

    ALsizei frameSize{FrameSizeFromDevFmt(device->FmtChans, device->FmtType, device->mAmbiOrder)};

    ALint64 done{0};
    auto start = std::chrono::steady_clock::now();
    while(!ATOMIC_LOAD(&self->killNow, almemory_order_acquire) &&
          ATOMIC_LOAD(&device->Connected, almemory_order_acquire))
    {
        auto now = std::chrono::steady_clock::now();

        /* This converts from nanoseconds to nanosamples, then to samples. */
        ALint64 avail{std::chrono::duration_cast<seconds>((now-start) * device->Frequency).count()};
        if(avail-done < device->UpdateSize)
        {
            std::this_thread::sleep_for(restTime);
            continue;
        }
        while(avail-done >= device->UpdateSize)
        {
            ALCwaveBackend_lock(self);
            aluMixData(device, self->mBuffer.data(), device->UpdateSize);
            ALCwaveBackend_unlock(self);
            done += device->UpdateSize;

            if(!IS_LITTLE_ENDIAN)
            {
                ALuint bytesize = BytesFromDevFmt(device->FmtType);
                ALuint i;

                if(bytesize == 2)
                {
                    ALushort *samples = reinterpret_cast<ALushort*>(self->mBuffer.data());
                    ALuint len = self->mBuffer.size() / 2;
                    for(i = 0;i < len;i++)
                    {
                        ALushort samp = samples[i];
                        samples[i] = (samp>>8) | (samp<<8);
                    }
                }
                else if(bytesize == 4)
                {
                    ALuint *samples = reinterpret_cast<ALuint*>(self->mBuffer.data());
                    ALuint len = self->mBuffer.size() / 4;
                    for(i = 0;i < len;i++)
                    {
                        ALuint samp = samples[i];
                        samples[i] = (samp>>24) | ((samp>>8)&0x0000ff00) |
                                     ((samp<<8)&0x00ff0000) | (samp<<24);
                    }
                }
            }

            size_t fs{fwrite(self->mBuffer.data(), frameSize, device->UpdateSize, self->mFile)};
            (void)fs;
            if(ferror(self->mFile))
            {
                ERR("Error writing to file\n");
                ALCwaveBackend_lock(self);
                aluHandleDisconnect(device, "Failed to write playback samples");
                ALCwaveBackend_unlock(self);
                break;
            }
        }

        /* For every completed second, increment the start time and reduce the
         * samples done. This prevents the difference between the start time
         * and current time from growing too large, while maintaining the
         * correct number of samples to render.
         */
        if(done >= device->Frequency)
        {
            seconds s{done/device->Frequency};
            start += s;
            done -= device->Frequency*s.count();
        }
    }

    return 0;
}


ALCenum ALCwaveBackend_open(ALCwaveBackend *self, const ALCchar *name)
{
    const char *fname{GetConfigValue(nullptr, "wave", "file", "")};
    if(!fname[0]) return ALC_INVALID_VALUE;

    if(!name)
        name = waveDevice;
    else if(strcmp(name, waveDevice) != 0)
        return ALC_INVALID_VALUE;

#ifdef _WIN32
    {
        std::wstring wname = utf8_to_wstr(fname);
        self->mFile = _wfopen(wname.c_str(), L"wb");
    }
#else
    self->mFile = fopen(fname, "wb");
#endif
    if(!self->mFile)
    {
        ERR("Could not open file '%s': %s\n", fname, strerror(errno));
        return ALC_INVALID_VALUE;
    }

    ALCdevice *device{STATIC_CAST(ALCbackend, self)->mDevice};
    device->DeviceName = name;

    return ALC_NO_ERROR;
}

ALCboolean ALCwaveBackend_reset(ALCwaveBackend *self)
{
    ALCdevice *device = STATIC_CAST(ALCbackend, self)->mDevice;
    ALuint channels=0, bits=0, chanmask=0;
    int isbformat = 0;
    size_t val;

    fseek(self->mFile, 0, SEEK_SET);
    clearerr(self->mFile);

    if(GetConfigValueBool(nullptr, "wave", "bformat", 0))
    {
        device->FmtChans = DevFmtAmbi3D;
        device->mAmbiOrder = 1;
    }

    switch(device->FmtType)
    {
        case DevFmtByte:
            device->FmtType = DevFmtUByte;
            break;
        case DevFmtUShort:
            device->FmtType = DevFmtShort;
            break;
        case DevFmtUInt:
            device->FmtType = DevFmtInt;
            break;
        case DevFmtUByte:
        case DevFmtShort:
        case DevFmtInt:
        case DevFmtFloat:
            break;
    }
    switch(device->FmtChans)
    {
        case DevFmtMono:   chanmask = 0x04; break;
        case DevFmtStereo: chanmask = 0x01 | 0x02; break;
        case DevFmtQuad:   chanmask = 0x01 | 0x02 | 0x10 | 0x20; break;
        case DevFmtX51: chanmask = 0x01 | 0x02 | 0x04 | 0x08 | 0x200 | 0x400; break;
        case DevFmtX51Rear: chanmask = 0x01 | 0x02 | 0x04 | 0x08 | 0x010 | 0x020; break;
        case DevFmtX61: chanmask = 0x01 | 0x02 | 0x04 | 0x08 | 0x100 | 0x200 | 0x400; break;
        case DevFmtX71: chanmask = 0x01 | 0x02 | 0x04 | 0x08 | 0x010 | 0x020 | 0x200 | 0x400; break;
        case DevFmtAmbi3D:
            /* .amb output requires FuMa */
            device->mAmbiLayout = AmbiLayout::FuMa;
            device->mAmbiScale = AmbiNorm::FuMa;
            isbformat = 1;
            chanmask = 0;
            break;
    }
    bits = BytesFromDevFmt(device->FmtType) * 8;
    channels = ChannelsFromDevFmt(device->FmtChans, device->mAmbiOrder);

    fputs("RIFF", self->mFile);
    fwrite32le(0xFFFFFFFF, self->mFile); // 'RIFF' header len; filled in at close

    fputs("WAVE", self->mFile);

    fputs("fmt ", self->mFile);
    fwrite32le(40, self->mFile); // 'fmt ' header len; 40 bytes for EXTENSIBLE

    // 16-bit val, format type id (extensible: 0xFFFE)
    fwrite16le(0xFFFE, self->mFile);
    // 16-bit val, channel count
    fwrite16le(channels, self->mFile);
    // 32-bit val, frequency
    fwrite32le(device->Frequency, self->mFile);
    // 32-bit val, bytes per second
    fwrite32le(device->Frequency * channels * bits / 8, self->mFile);
    // 16-bit val, frame size
    fwrite16le(channels * bits / 8, self->mFile);
    // 16-bit val, bits per sample
    fwrite16le(bits, self->mFile);
    // 16-bit val, extra byte count
    fwrite16le(22, self->mFile);
    // 16-bit val, valid bits per sample
    fwrite16le(bits, self->mFile);
    // 32-bit val, channel mask
    fwrite32le(chanmask, self->mFile);
    // 16 byte GUID, sub-type format
    val = fwrite((device->FmtType == DevFmtFloat) ?
                 (isbformat ? SUBTYPE_BFORMAT_FLOAT : SUBTYPE_FLOAT) :
                 (isbformat ? SUBTYPE_BFORMAT_PCM : SUBTYPE_PCM), 1, 16, self->mFile);
    (void)val;

    fputs("data", self->mFile);
    fwrite32le(0xFFFFFFFF, self->mFile); // 'data' header len; filled in at close

    if(ferror(self->mFile))
    {
        ERR("Error writing header: %s\n", strerror(errno));
        return ALC_FALSE;
    }
    self->mDataStart = ftell(self->mFile);

    SetDefaultWFXChannelOrder(device);

    ALuint bufsize{FrameSizeFromDevFmt(
        device->FmtChans, device->FmtType, device->mAmbiOrder
    ) * device->UpdateSize};
    self->mBuffer.resize(bufsize);

    return ALC_TRUE;
}

ALCboolean ALCwaveBackend_start(ALCwaveBackend *self)
{
    try {
        ATOMIC_STORE(&self->killNow, AL_FALSE, almemory_order_release);
        self->thread = std::thread(ALCwaveBackend_mixerProc, self);
        return ALC_TRUE;
    }
    catch(std::exception& e) {
        ERR("Failed to start mixing thread: %s\n", e.what());
    }
    catch(...) {
    }
    return ALC_FALSE;
}

void ALCwaveBackend_stop(ALCwaveBackend *self)
{
    if(self->killNow.exchange(AL_TRUE, std::memory_order_acq_rel) || !self->thread.joinable())
        return;
    self->thread.join();

    long size{ftell(self->mFile)};
    if(size > 0)
    {
        long dataLen{size - self->mDataStart};
        if(fseek(self->mFile, self->mDataStart-4, SEEK_SET) == 0)
            fwrite32le(dataLen, self->mFile); // 'data' header len
        if(fseek(self->mFile, 4, SEEK_SET) == 0)
            fwrite32le(size-8, self->mFile); // 'WAVE' header len
    }
}

} // namespace


bool WaveBackendFactory::init()
{ return true; }

bool WaveBackendFactory::querySupport(ALCbackend_Type type)
{ return (type == ALCbackend_Playback); }

void WaveBackendFactory::probe(enum DevProbe type, std::string *outnames)
{
    switch(type)
    {
        case ALL_DEVICE_PROBE:
            /* Includes null char. */
            outnames->append(waveDevice, sizeof(waveDevice));
            break;
        case CAPTURE_DEVICE_PROBE:
            break;
    }
}

ALCbackend *WaveBackendFactory::createBackend(ALCdevice *device, ALCbackend_Type type)
{
    if(type == ALCbackend_Playback)
    {
        ALCwaveBackend *backend;
        NEW_OBJ(backend, ALCwaveBackend)(device);
        if(!backend) return nullptr;
        return STATIC_CAST(ALCbackend, backend);
    }

    return nullptr;
}

BackendFactory &WaveBackendFactory::getFactory()
{
    static WaveBackendFactory factory{};
    return factory;
}
