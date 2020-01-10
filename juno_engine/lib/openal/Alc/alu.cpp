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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <algorithm>

#include "alMain.h"
#include "alcontext.h"
#include "alSource.h"
#include "alBuffer.h"
#include "alListener.h"
#include "alAuxEffectSlot.h"
#include "alu.h"
#include "bs2b.h"
#include "hrtf.h"
#include "mastering.h"
#include "uhjfilter.h"
#include "bformatdec.h"
#include "ringbuffer.h"
#include "filters/splitter.h"

#include "mixer/defs.h"
#include "fpu_modes.h"
#include "cpu_caps.h"
#include "bsinc_inc.h"


/* Cone scalar */
ALfloat ConeScale = 1.0f;

/* Localized Z scalar for mono sources */
ALfloat ZScale = 1.0f;

/* Force default speed of sound for distance-related reverb decay. */
ALboolean OverrideReverbSpeedOfSound = AL_FALSE;


namespace {

void ClearArray(ALfloat f[MAX_OUTPUT_CHANNELS])
{
    size_t i;
    for(i = 0;i < MAX_OUTPUT_CHANNELS;i++)
        f[i] = 0.0f;
}

struct ChanMap {
    enum Channel channel;
    ALfloat angle;
    ALfloat elevation;
};

HrtfDirectMixerFunc MixDirectHrtf = MixDirectHrtf_C;


inline HrtfDirectMixerFunc SelectHrtfMixer(void)
{
#ifdef HAVE_NEON
    if((CPUCapFlags&CPU_CAP_NEON))
        return MixDirectHrtf_Neon;
#endif
#ifdef HAVE_SSE
    if((CPUCapFlags&CPU_CAP_SSE))
        return MixDirectHrtf_SSE;
#endif

    return MixDirectHrtf_C;
}

} // namespace

void aluInit(void)
{
    MixDirectHrtf = SelectHrtfMixer();
}


void DeinitVoice(ALvoice *voice)
{
    al_free(voice->Update.exchange(nullptr));
}


namespace {

void ProcessHrtf(ALCdevice *device, ALsizei SamplesToDo)
{
    if(device->AmbiUp)
        ambiup_process(device->AmbiUp.get(),
            device->Dry.Buffer, device->Dry.NumChannels, device->FOAOut.Buffer,
            SamplesToDo
        );

    int lidx{GetChannelIdxByName(&device->RealOut, FrontLeft)};
    int ridx{GetChannelIdxByName(&device->RealOut, FrontRight)};
    assert(lidx != -1 && ridx != -1);

    DirectHrtfState *state{device->mHrtfState.get()};
    for(ALsizei c{0};c < device->Dry.NumChannels;c++)
    {
        MixDirectHrtf(device->RealOut.Buffer[lidx], device->RealOut.Buffer[ridx],
            device->Dry.Buffer[c], state->Offset, state->IrSize,
            state->Chan[c].Coeffs, state->Chan[c].Values, SamplesToDo
        );
    }
    state->Offset += SamplesToDo;
}

void ProcessAmbiDec(ALCdevice *device, ALsizei SamplesToDo)
{
    if(device->Dry.Buffer != device->FOAOut.Buffer)
        bformatdec_upSample(device->AmbiDecoder.get(),
            device->Dry.Buffer, device->FOAOut.Buffer, device->FOAOut.NumChannels,
            SamplesToDo
        );
    bformatdec_process(device->AmbiDecoder.get(),
        device->RealOut.Buffer, device->RealOut.NumChannels, device->Dry.Buffer,
        SamplesToDo
    );
}

void ProcessAmbiUp(ALCdevice *device, ALsizei SamplesToDo)
{
    ambiup_process(device->AmbiUp.get(),
        device->RealOut.Buffer, device->RealOut.NumChannels, device->FOAOut.Buffer,
        SamplesToDo
    );
}

void ProcessUhj(ALCdevice *device, ALsizei SamplesToDo)
{
    int lidx = GetChannelIdxByName(&device->RealOut, FrontLeft);
    int ridx = GetChannelIdxByName(&device->RealOut, FrontRight);
    assert(lidx != -1 && ridx != -1);

    /* Encode to stereo-compatible 2-channel UHJ output. */
    EncodeUhj2(device->Uhj_Encoder.get(),
        device->RealOut.Buffer[lidx], device->RealOut.Buffer[ridx],
        device->Dry.Buffer, SamplesToDo
    );
}

void ProcessBs2b(ALCdevice *device, ALsizei SamplesToDo)
{
    int lidx = GetChannelIdxByName(&device->RealOut, FrontLeft);
    int ridx = GetChannelIdxByName(&device->RealOut, FrontRight);
    assert(lidx != -1 && ridx != -1);

    /* Apply binaural/crossfeed filter */
    bs2b_cross_feed(device->Bs2b.get(), device->RealOut.Buffer[lidx],
                    device->RealOut.Buffer[ridx], SamplesToDo);
}

} // namespace

void aluSelectPostProcess(ALCdevice *device)
{
    if(device->HrtfHandle)
        device->PostProcess = ProcessHrtf;
    else if(device->AmbiDecoder)
        device->PostProcess = ProcessAmbiDec;
    else if(device->AmbiUp)
        device->PostProcess = ProcessAmbiUp;
    else if(device->Uhj_Encoder)
        device->PostProcess = ProcessUhj;
    else if(device->Bs2b)
        device->PostProcess = ProcessBs2b;
    else
        device->PostProcess = NULL;
}


/* Prepares the interpolator for a given rate (determined by increment).
 *
 * With a bit of work, and a trade of memory for CPU cost, this could be
 * modified for use with an interpolated increment for buttery-smooth pitch
 * changes.
 */
void BsincPrepare(const ALuint increment, BsincState *state, const BSincTable *table)
{
    ALfloat sf = 0.0f;
    ALsizei si = BSINC_SCALE_COUNT-1;

    if(increment > FRACTIONONE)
    {
        sf = (ALfloat)FRACTIONONE / increment;
        sf = maxf(0.0f, (BSINC_SCALE_COUNT-1) * (sf-table->scaleBase) * table->scaleRange);
        si = float2int(sf);
        /* The interpolation factor is fit to this diagonally-symmetric curve
         * to reduce the transition ripple caused by interpolating different
         * scales of the sinc function.
         */
        sf = 1.0f - cosf(asinf(sf - si));
    }

    state->sf = sf;
    state->m = table->m[si];
    state->l = (state->m/2) - 1;
    state->filter = table->Tab + table->filterOffset[si];
}


namespace {

/* This RNG method was created based on the math found in opusdec. It's quick,
 * and starting with a seed value of 22222, is suitable for generating
 * whitenoise.
 */
inline ALuint dither_rng(ALuint *seed) noexcept
{
    *seed = (*seed * 96314165) + 907633515;
    return *seed;
}


inline void aluCrossproduct(const ALfloat *inVector1, const ALfloat *inVector2, ALfloat *outVector)
{
    outVector[0] = inVector1[1]*inVector2[2] - inVector1[2]*inVector2[1];
    outVector[1] = inVector1[2]*inVector2[0] - inVector1[0]*inVector2[2];
    outVector[2] = inVector1[0]*inVector2[1] - inVector1[1]*inVector2[0];
}

inline ALfloat aluDotproduct(const aluVector *vec1, const aluVector *vec2)
{
    return vec1->v[0]*vec2->v[0] + vec1->v[1]*vec2->v[1] + vec1->v[2]*vec2->v[2];
}

ALfloat aluNormalize(ALfloat *vec)
{
    ALfloat length = sqrtf(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    if(length > FLT_EPSILON)
    {
        ALfloat inv_length = 1.0f/length;
        vec[0] *= inv_length;
        vec[1] *= inv_length;
        vec[2] *= inv_length;
        return length;
    }
    vec[0] = vec[1] = vec[2] = 0.0f;
    return 0.0f;
}

void aluMatrixfFloat3(ALfloat *vec, ALfloat w, const aluMatrixf *mtx)
{
    ALfloat v[4] = { vec[0], vec[1], vec[2], w };

    vec[0] = v[0]*mtx->m[0][0] + v[1]*mtx->m[1][0] + v[2]*mtx->m[2][0] + v[3]*mtx->m[3][0];
    vec[1] = v[0]*mtx->m[0][1] + v[1]*mtx->m[1][1] + v[2]*mtx->m[2][1] + v[3]*mtx->m[3][1];
    vec[2] = v[0]*mtx->m[0][2] + v[1]*mtx->m[1][2] + v[2]*mtx->m[2][2] + v[3]*mtx->m[3][2];
}

aluVector aluMatrixfVector(const aluMatrixf *mtx, const aluVector *vec)
{
    aluVector v;
    v.v[0] = vec->v[0]*mtx->m[0][0] + vec->v[1]*mtx->m[1][0] + vec->v[2]*mtx->m[2][0] + vec->v[3]*mtx->m[3][0];
    v.v[1] = vec->v[0]*mtx->m[0][1] + vec->v[1]*mtx->m[1][1] + vec->v[2]*mtx->m[2][1] + vec->v[3]*mtx->m[3][1];
    v.v[2] = vec->v[0]*mtx->m[0][2] + vec->v[1]*mtx->m[1][2] + vec->v[2]*mtx->m[2][2] + vec->v[3]*mtx->m[3][2];
    v.v[3] = vec->v[0]*mtx->m[0][3] + vec->v[1]*mtx->m[1][3] + vec->v[2]*mtx->m[2][3] + vec->v[3]*mtx->m[3][3];
    return v;
}


void SendSourceStoppedEvent(ALCcontext *context, ALuint id)
{
    AsyncEvent evt = ASYNC_EVENT(EventType_SourceStateChange);
    ALbitfieldSOFT enabledevt;
    size_t strpos;
    ALuint scale;

    enabledevt = context->EnabledEvts.load(std::memory_order_acquire);
    if(!(enabledevt&EventType_SourceStateChange)) return;

    evt.u.user.type = AL_EVENT_TYPE_SOURCE_STATE_CHANGED_SOFT;
    evt.u.user.id = id;
    evt.u.user.param = AL_STOPPED;

    /* Normally snprintf would be used, but this is called from the mixer and
     * that function's not real-time safe, so we have to construct it manually.
     */
    strcpy(evt.u.user.msg, "Source ID "); strpos = 10;
    scale = 1000000000;
    while(scale > 0 && scale > id)
        scale /= 10;
    while(scale > 0)
    {
        evt.u.user.msg[strpos++] = '0' + ((id/scale)%10);
        scale /= 10;
    }
    strcpy(evt.u.user.msg+strpos, " state changed to AL_STOPPED");

    if(ll_ringbuffer_write(context->AsyncEvents, &evt, 1) == 1)
        alsem_post(&context->EventSem);
}


bool CalcContextParams(ALCcontext *Context)
{
    ALlistener &Listener = Context->Listener;
    struct ALcontextProps *props;

    props = Context->Update.exchange(nullptr, std::memory_order_acq_rel);
    if(!props) return false;

    Listener.Params.MetersPerUnit = props->MetersPerUnit;

    Listener.Params.DopplerFactor = props->DopplerFactor;
    Listener.Params.SpeedOfSound = props->SpeedOfSound * props->DopplerVelocity;
    if(!OverrideReverbSpeedOfSound)
        Listener.Params.ReverbSpeedOfSound = Listener.Params.SpeedOfSound *
                                             Listener.Params.MetersPerUnit;

    Listener.Params.SourceDistanceModel = props->SourceDistanceModel;
    Listener.Params.mDistanceModel = props->mDistanceModel;

    AtomicReplaceHead(Context->FreeContextProps, props);
    return true;
}

bool CalcListenerParams(ALCcontext *Context)
{
    ALlistener &Listener = Context->Listener;
    ALfloat N[3], V[3], U[3], P[3];
    struct ALlistenerProps *props;
    aluVector vel;

    props = Listener.Update.exchange(nullptr, std::memory_order_acq_rel);
    if(!props) return false;

    /* AT then UP */
    N[0] = props->Forward[0];
    N[1] = props->Forward[1];
    N[2] = props->Forward[2];
    aluNormalize(N);
    V[0] = props->Up[0];
    V[1] = props->Up[1];
    V[2] = props->Up[2];
    aluNormalize(V);
    /* Build and normalize right-vector */
    aluCrossproduct(N, V, U);
    aluNormalize(U);

    aluMatrixfSet(&Listener.Params.Matrix,
        U[0], V[0], -N[0], 0.0,
        U[1], V[1], -N[1], 0.0,
        U[2], V[2], -N[2], 0.0,
         0.0,  0.0,   0.0, 1.0
    );

    P[0] = props->Position[0];
    P[1] = props->Position[1];
    P[2] = props->Position[2];
    aluMatrixfFloat3(P, 1.0, &Listener.Params.Matrix);
    aluMatrixfSetRow(&Listener.Params.Matrix, 3, -P[0], -P[1], -P[2], 1.0f);

    aluVectorSet(&vel, props->Velocity[0], props->Velocity[1], props->Velocity[2], 0.0f);
    Listener.Params.Velocity = aluMatrixfVector(&Listener.Params.Matrix, &vel);

    Listener.Params.Gain = props->Gain * Context->GainBoost;

    AtomicReplaceHead(Context->FreeListenerProps, props);
    return true;
}

bool CalcEffectSlotParams(ALeffectslot *slot, ALCcontext *context, bool force)
{
    struct ALeffectslotProps *props;
    EffectState *state;

    props = slot->Update.exchange(nullptr, std::memory_order_acq_rel);
    if(!props && !force) return false;

    if(props)
    {
        slot->Params.Gain = props->Gain;
        slot->Params.AuxSendAuto = props->AuxSendAuto;
        slot->Params.EffectType = props->Type;
        slot->Params.EffectProps = props->Props;
        if(IsReverbEffect(props->Type))
        {
            slot->Params.RoomRolloff = props->Props.Reverb.RoomRolloffFactor;
            slot->Params.DecayTime = props->Props.Reverb.DecayTime;
            slot->Params.DecayLFRatio = props->Props.Reverb.DecayLFRatio;
            slot->Params.DecayHFRatio = props->Props.Reverb.DecayHFRatio;
            slot->Params.DecayHFLimit = props->Props.Reverb.DecayHFLimit;
            slot->Params.AirAbsorptionGainHF = props->Props.Reverb.AirAbsorptionGainHF;
        }
        else
        {
            slot->Params.RoomRolloff = 0.0f;
            slot->Params.DecayTime = 0.0f;
            slot->Params.DecayLFRatio = 0.0f;
            slot->Params.DecayHFRatio = 0.0f;
            slot->Params.DecayHFLimit = AL_FALSE;
            slot->Params.AirAbsorptionGainHF = 1.0f;
        }

        state = props->State;

        if(state == slot->Params.mEffectState)
        {
            /* If the effect state is the same as current, we can decrement its
             * count safely to remove it from the update object (it can't reach
             * 0 refs since the current params also hold a reference).
             */
            DecrementRef(&state->mRef);
            props->State = nullptr;
        }
        else
        {
            /* Otherwise, replace it and send off the old one with a release
             * event.
             */
            AsyncEvent evt = ASYNC_EVENT(EventType_ReleaseEffectState);
            evt.u.mEffectState = slot->Params.mEffectState;

            slot->Params.mEffectState = state;
            props->State = NULL;

            if(LIKELY(ll_ringbuffer_write(context->AsyncEvents, &evt, 1) != 0))
                alsem_post(&context->EventSem);
            else
            {
                /* If writing the event failed, the queue was probably full.
                 * Store the old state in the property object where it can
                 * eventually be cleaned up sometime later (not ideal, but
                 * better than blocking or leaking).
                 */
                props->State = evt.u.mEffectState;
            }
        }

        AtomicReplaceHead(context->FreeEffectslotProps, props);
    }
    else
        state = slot->Params.mEffectState;

    state->update(context, slot, &slot->Params.EffectProps);
    return true;
}


constexpr struct ChanMap MonoMap[1] = {
    { FrontCenter, 0.0f, 0.0f }
}, RearMap[2] = {
    { BackLeft,  DEG2RAD(-150.0f), DEG2RAD(0.0f) },
    { BackRight, DEG2RAD( 150.0f), DEG2RAD(0.0f) }
}, QuadMap[4] = {
    { FrontLeft,  DEG2RAD( -45.0f), DEG2RAD(0.0f) },
    { FrontRight, DEG2RAD(  45.0f), DEG2RAD(0.0f) },
    { BackLeft,   DEG2RAD(-135.0f), DEG2RAD(0.0f) },
    { BackRight,  DEG2RAD( 135.0f), DEG2RAD(0.0f) }
}, X51Map[6] = {
    { FrontLeft,   DEG2RAD( -30.0f), DEG2RAD(0.0f) },
    { FrontRight,  DEG2RAD(  30.0f), DEG2RAD(0.0f) },
    { FrontCenter, DEG2RAD(   0.0f), DEG2RAD(0.0f) },
    { LFE, 0.0f, 0.0f },
    { SideLeft,    DEG2RAD(-110.0f), DEG2RAD(0.0f) },
    { SideRight,   DEG2RAD( 110.0f), DEG2RAD(0.0f) }
}, X61Map[7] = {
    { FrontLeft,    DEG2RAD(-30.0f), DEG2RAD(0.0f) },
    { FrontRight,   DEG2RAD( 30.0f), DEG2RAD(0.0f) },
    { FrontCenter,  DEG2RAD(  0.0f), DEG2RAD(0.0f) },
    { LFE, 0.0f, 0.0f },
    { BackCenter,   DEG2RAD(180.0f), DEG2RAD(0.0f) },
    { SideLeft,     DEG2RAD(-90.0f), DEG2RAD(0.0f) },
    { SideRight,    DEG2RAD( 90.0f), DEG2RAD(0.0f) }
}, X71Map[8] = {
    { FrontLeft,   DEG2RAD( -30.0f), DEG2RAD(0.0f) },
    { FrontRight,  DEG2RAD(  30.0f), DEG2RAD(0.0f) },
    { FrontCenter, DEG2RAD(   0.0f), DEG2RAD(0.0f) },
    { LFE, 0.0f, 0.0f },
    { BackLeft,    DEG2RAD(-150.0f), DEG2RAD(0.0f) },
    { BackRight,   DEG2RAD( 150.0f), DEG2RAD(0.0f) },
    { SideLeft,    DEG2RAD( -90.0f), DEG2RAD(0.0f) },
    { SideRight,   DEG2RAD(  90.0f), DEG2RAD(0.0f) }
};

void CalcPanningAndFilters(ALvoice *voice, const ALfloat Azi, const ALfloat Elev,
                           const ALfloat Distance, const ALfloat Spread,
                           const ALfloat DryGain, const ALfloat DryGainHF,
                           const ALfloat DryGainLF, const ALfloat *WetGain,
                           const ALfloat *WetGainLF, const ALfloat *WetGainHF,
                           ALeffectslot **SendSlots, const ALbuffer *Buffer,
                           const struct ALvoiceProps *props, const ALlistener &Listener,
                           const ALCdevice *Device)
{
    struct ChanMap StereoMap[2] = {
        { FrontLeft,  DEG2RAD(-30.0f), DEG2RAD(0.0f) },
        { FrontRight, DEG2RAD( 30.0f), DEG2RAD(0.0f) }
    };
    bool DirectChannels = props->DirectChannels;
    const ALsizei NumSends = Device->NumAuxSends;
    const ALuint Frequency = Device->Frequency;
    const struct ChanMap *chans = NULL;
    ALsizei num_channels = 0;
    bool isbformat = false;
    ALfloat downmix_gain = 1.0f;
    ALsizei c, i;

    switch(Buffer->FmtChannels)
    {
    case FmtMono:
        chans = MonoMap;
        num_channels = 1;
        /* Mono buffers are never played direct. */
        DirectChannels = false;
        break;

    case FmtStereo:
        /* Convert counter-clockwise to clockwise. */
        StereoMap[0].angle = -props->StereoPan[0];
        StereoMap[1].angle = -props->StereoPan[1];

        chans = StereoMap;
        num_channels = 2;
        downmix_gain = 1.0f / 2.0f;
        break;

    case FmtRear:
        chans = RearMap;
        num_channels = 2;
        downmix_gain = 1.0f / 2.0f;
        break;

    case FmtQuad:
        chans = QuadMap;
        num_channels = 4;
        downmix_gain = 1.0f / 4.0f;
        break;

    case FmtX51:
        chans = X51Map;
        num_channels = 6;
        /* NOTE: Excludes LFE. */
        downmix_gain = 1.0f / 5.0f;
        break;

    case FmtX61:
        chans = X61Map;
        num_channels = 7;
        /* NOTE: Excludes LFE. */
        downmix_gain = 1.0f / 6.0f;
        break;

    case FmtX71:
        chans = X71Map;
        num_channels = 8;
        /* NOTE: Excludes LFE. */
        downmix_gain = 1.0f / 7.0f;
        break;

    case FmtBFormat2D:
        num_channels = 3;
        isbformat = true;
        DirectChannels = false;
        break;

    case FmtBFormat3D:
        num_channels = 4;
        isbformat = true;
        DirectChannels = false;
        break;
    }

    for(c = 0;c < num_channels;c++)
    {
        memset(&voice->Direct.Params[c].Hrtf.Target, 0,
               sizeof(voice->Direct.Params[c].Hrtf.Target));
        ClearArray(voice->Direct.Params[c].Gains.Target);
    }
    for(i = 0;i < NumSends;i++)
    {
        for(c = 0;c < num_channels;c++)
            ClearArray(voice->Send[i].Params[c].Gains.Target);
    }

    voice->Flags &= ~(VOICE_HAS_HRTF | VOICE_HAS_NFC);
    if(isbformat)
    {
        /* Special handling for B-Format sources. */

        if(Distance > FLT_EPSILON)
        {
            /* Panning a B-Format sound toward some direction is easy. Just pan
             * the first (W) channel as a normal mono sound and silence the
             * others.
             */
            ALfloat coeffs[MAX_AMBI_COEFFS];

            if(Device->AvgSpeakerDist > 0.0f)
            {
                ALfloat mdist = Distance * Listener.Params.MetersPerUnit;
                ALfloat w0 = SPEEDOFSOUNDMETRESPERSEC /
                             (mdist * (ALfloat)Device->Frequency);
                ALfloat w1 = SPEEDOFSOUNDMETRESPERSEC /
                             (Device->AvgSpeakerDist * (ALfloat)Device->Frequency);
                /* Clamp w0 for really close distances, to prevent excessive
                 * bass.
                 */
                w0 = minf(w0, w1*4.0f);

                /* Only need to adjust the first channel of a B-Format source. */
                NfcFilterAdjust(&voice->Direct.Params[0].NFCtrlFilter, w0);

                for(i = 0;i < MAX_AMBI_ORDER+1;i++)
                    voice->Direct.ChannelsPerOrder[i] = Device->NumChannelsPerOrder[i];
                voice->Flags |= VOICE_HAS_NFC;
            }

            /* A scalar of 1.5 for plain stereo results in +/-60 degrees being
             * moved to +/-90 degrees for direct right and left speaker
             * responses.
             */
            CalcAngleCoeffs((Device->Render_Mode==StereoPair) ? ScaleAzimuthFront(Azi, 1.5f) : Azi,
                            Elev, Spread, coeffs);

            /* NOTE: W needs to be scaled by sqrt(2) due to FuMa normalization. */
            ComputePanGains(&Device->Dry, coeffs, DryGain*SQRTF_2,
                               voice->Direct.Params[0].Gains.Target);
            for(i = 0;i < NumSends;i++)
            {
                const ALeffectslot *Slot = SendSlots[i];
                if(Slot)
                    ComputePanningGainsBF(Slot->ChanMap, Slot->NumChannels, coeffs,
                        WetGain[i]*SQRTF_2, voice->Send[i].Params[0].Gains.Target
                    );
            }
        }
        else
        {
            /* Local B-Format sources have their XYZ channels rotated according
             * to the orientation.
             */
            ALfloat N[3], V[3], U[3];
            aluMatrixf matrix;

            if(Device->AvgSpeakerDist > 0.0f)
            {
                /* NOTE: The NFCtrlFilters were created with a w0 of 0, which
                 * is what we want for FOA input. The first channel may have
                 * been previously re-adjusted if panned, so reset it.
                 */
                NfcFilterAdjust(&voice->Direct.Params[0].NFCtrlFilter, 0.0f);

                voice->Direct.ChannelsPerOrder[0] = 1;
                voice->Direct.ChannelsPerOrder[1] = mini(voice->Direct.Channels-1, 3);
                for(i = 2;i < MAX_AMBI_ORDER+1;i++)
                    voice->Direct.ChannelsPerOrder[i] = 0;
                voice->Flags |= VOICE_HAS_NFC;
            }

            /* AT then UP */
            N[0] = props->Orientation[0][0];
            N[1] = props->Orientation[0][1];
            N[2] = props->Orientation[0][2];
            aluNormalize(N);
            V[0] = props->Orientation[1][0];
            V[1] = props->Orientation[1][1];
            V[2] = props->Orientation[1][2];
            aluNormalize(V);
            if(!props->HeadRelative)
            {
                const aluMatrixf *lmatrix = &Listener.Params.Matrix;
                aluMatrixfFloat3(N, 0.0f, lmatrix);
                aluMatrixfFloat3(V, 0.0f, lmatrix);
            }
            /* Build and normalize right-vector */
            aluCrossproduct(N, V, U);
            aluNormalize(U);

            /* Build a rotate + conversion matrix (FuMa -> ACN+N3D). NOTE: This
             * matrix is transposed, for the inputs to align on the rows and
             * outputs on the columns.
             */
            aluMatrixfSet(&matrix,
                // ACN0           ACN1           ACN2           ACN3
                SQRTF_2,          0.0f,          0.0f,          0.0f, // Ambi W
                   0.0f, -N[0]*SQRTF_3,  N[1]*SQRTF_3, -N[2]*SQRTF_3, // Ambi X
                   0.0f,  U[0]*SQRTF_3, -U[1]*SQRTF_3,  U[2]*SQRTF_3, // Ambi Y
                   0.0f, -V[0]*SQRTF_3,  V[1]*SQRTF_3, -V[2]*SQRTF_3  // Ambi Z
            );

            voice->Direct.Buffer = Device->FOAOut.Buffer;
            voice->Direct.Channels = Device->FOAOut.NumChannels;
            for(c = 0;c < num_channels;c++)
                ComputePanGains(&Device->FOAOut, matrix.m[c], DryGain,
                                voice->Direct.Params[c].Gains.Target);
            for(i = 0;i < NumSends;i++)
            {
                const ALeffectslot *Slot = SendSlots[i];
                if(Slot)
                {
                    for(c = 0;c < num_channels;c++)
                        ComputePanningGainsBF(Slot->ChanMap, Slot->NumChannels,
                            matrix.m[c], WetGain[i], voice->Send[i].Params[c].Gains.Target
                        );
                }
            }
        }
    }
    else if(DirectChannels)
    {
        /* Direct source channels always play local. Skip the virtual channels
         * and write inputs to the matching real outputs.
         */
        voice->Direct.Buffer = Device->RealOut.Buffer;
        voice->Direct.Channels = Device->RealOut.NumChannels;

        for(c = 0;c < num_channels;c++)
        {
            int idx = GetChannelIdxByName(&Device->RealOut, chans[c].channel);
            if(idx != -1) voice->Direct.Params[c].Gains.Target[idx] = DryGain;
        }

        /* Auxiliary sends still use normal channel panning since they mix to
         * B-Format, which can't channel-match.
         */
        for(c = 0;c < num_channels;c++)
        {
            ALfloat coeffs[MAX_AMBI_COEFFS];
            CalcAngleCoeffs(chans[c].angle, chans[c].elevation, 0.0f, coeffs);

            for(i = 0;i < NumSends;i++)
            {
                const ALeffectslot *Slot = SendSlots[i];
                if(Slot)
                    ComputePanningGainsBF(Slot->ChanMap, Slot->NumChannels,
                        coeffs, WetGain[i], voice->Send[i].Params[c].Gains.Target
                    );
            }
        }
    }
    else if(Device->Render_Mode == HrtfRender)
    {
        /* Full HRTF rendering. Skip the virtual channels and render to the
         * real outputs.
         */
        voice->Direct.Buffer = Device->RealOut.Buffer;
        voice->Direct.Channels = Device->RealOut.NumChannels;

        if(Distance > FLT_EPSILON)
        {
            ALfloat coeffs[MAX_AMBI_COEFFS];

            /* Get the HRIR coefficients and delays just once, for the given
             * source direction.
             */
            GetHrtfCoeffs(Device->HrtfHandle, Elev, Azi, Spread,
                          voice->Direct.Params[0].Hrtf.Target.Coeffs,
                          voice->Direct.Params[0].Hrtf.Target.Delay);
            voice->Direct.Params[0].Hrtf.Target.Gain = DryGain * downmix_gain;

            /* Remaining channels use the same results as the first. */
            for(c = 1;c < num_channels;c++)
            {
                /* Skip LFE */
                if(chans[c].channel != LFE)
                    voice->Direct.Params[c].Hrtf.Target = voice->Direct.Params[0].Hrtf.Target;
            }

            /* Calculate the directional coefficients once, which apply to all
             * input channels of the source sends.
             */
            CalcAngleCoeffs(Azi, Elev, Spread, coeffs);

            for(i = 0;i < NumSends;i++)
            {
                const ALeffectslot *Slot = SendSlots[i];
                if(Slot)
                    for(c = 0;c < num_channels;c++)
                    {
                        /* Skip LFE */
                        if(chans[c].channel != LFE)
                            ComputePanningGainsBF(Slot->ChanMap,
                                Slot->NumChannels, coeffs, WetGain[i] * downmix_gain,
                                voice->Send[i].Params[c].Gains.Target
                            );
                    }
            }
        }
        else
        {
            /* Local sources on HRTF play with each channel panned to its
             * relative location around the listener, providing "virtual
             * speaker" responses.
             */
            for(c = 0;c < num_channels;c++)
            {
                ALfloat coeffs[MAX_AMBI_COEFFS];

                if(chans[c].channel == LFE)
                {
                    /* Skip LFE */
                    continue;
                }

                /* Get the HRIR coefficients and delays for this channel
                 * position.
                 */
                GetHrtfCoeffs(Device->HrtfHandle,
                    chans[c].elevation, chans[c].angle, Spread,
                    voice->Direct.Params[c].Hrtf.Target.Coeffs,
                    voice->Direct.Params[c].Hrtf.Target.Delay
                );
                voice->Direct.Params[c].Hrtf.Target.Gain = DryGain;

                /* Normal panning for auxiliary sends. */
                CalcAngleCoeffs(chans[c].angle, chans[c].elevation, Spread, coeffs);

                for(i = 0;i < NumSends;i++)
                {
                    const ALeffectslot *Slot = SendSlots[i];
                    if(Slot)
                        ComputePanningGainsBF(Slot->ChanMap, Slot->NumChannels,
                            coeffs, WetGain[i], voice->Send[i].Params[c].Gains.Target
                        );
                }
            }
        }

        voice->Flags |= VOICE_HAS_HRTF;
    }
    else
    {
        /* Non-HRTF rendering. Use normal panning to the output. */

        if(Distance > FLT_EPSILON)
        {
            ALfloat coeffs[MAX_AMBI_COEFFS];
            ALfloat w0 = 0.0f;

            /* Calculate NFC filter coefficient if needed. */
            if(Device->AvgSpeakerDist > 0.0f)
            {
                ALfloat mdist = Distance * Listener.Params.MetersPerUnit;
                ALfloat w1 = SPEEDOFSOUNDMETRESPERSEC /
                             (Device->AvgSpeakerDist * (ALfloat)Device->Frequency);
                w0 = SPEEDOFSOUNDMETRESPERSEC /
                     (mdist * (ALfloat)Device->Frequency);
                /* Clamp w0 for really close distances, to prevent excessive
                 * bass.
                 */
                w0 = minf(w0, w1*4.0f);

                /* Adjust NFC filters. */
                for(c = 0;c < num_channels;c++)
                    NfcFilterAdjust(&voice->Direct.Params[c].NFCtrlFilter, w0);

                for(i = 0;i < MAX_AMBI_ORDER+1;i++)
                    voice->Direct.ChannelsPerOrder[i] = Device->NumChannelsPerOrder[i];
                voice->Flags |= VOICE_HAS_NFC;
            }

            /* Calculate the directional coefficients once, which apply to all
             * input channels.
             */
            CalcAngleCoeffs((Device->Render_Mode==StereoPair) ? ScaleAzimuthFront(Azi, 1.5f) : Azi,
                            Elev, Spread, coeffs);

            for(c = 0;c < num_channels;c++)
            {
                /* Special-case LFE */
                if(chans[c].channel == LFE)
                {
                    if(Device->Dry.Buffer == Device->RealOut.Buffer)
                    {
                        int idx = GetChannelIdxByName(&Device->RealOut, chans[c].channel);
                        if(idx != -1) voice->Direct.Params[c].Gains.Target[idx] = DryGain;
                    }
                    continue;
                }

                ComputePanGains(&Device->Dry, coeffs, DryGain * downmix_gain,
                                voice->Direct.Params[c].Gains.Target);
            }

            for(i = 0;i < NumSends;i++)
            {
                const ALeffectslot *Slot = SendSlots[i];
                if(Slot)
                    for(c = 0;c < num_channels;c++)
                    {
                        /* Skip LFE */
                        if(chans[c].channel != LFE)
                            ComputePanningGainsBF(Slot->ChanMap,
                                Slot->NumChannels, coeffs, WetGain[i] * downmix_gain,
                                voice->Send[i].Params[c].Gains.Target
                            );
                    }
            }
        }
        else
        {
            ALfloat w0 = 0.0f;

            if(Device->AvgSpeakerDist > 0.0f)
            {
                /* If the source distance is 0, set w0 to w1 to act as a pass-
                 * through. We still want to pass the signal through the
                 * filters so they keep an appropriate history, in case the
                 * source moves away from the listener.
                 */
                w0 = SPEEDOFSOUNDMETRESPERSEC /
                     (Device->AvgSpeakerDist * (ALfloat)Device->Frequency);

                for(c = 0;c < num_channels;c++)
                    NfcFilterAdjust(&voice->Direct.Params[c].NFCtrlFilter, w0);

                for(i = 0;i < MAX_AMBI_ORDER+1;i++)
                    voice->Direct.ChannelsPerOrder[i] = Device->NumChannelsPerOrder[i];
                voice->Flags |= VOICE_HAS_NFC;
            }

            for(c = 0;c < num_channels;c++)
            {
                ALfloat coeffs[MAX_AMBI_COEFFS];

                /* Special-case LFE */
                if(chans[c].channel == LFE)
                {
                    if(Device->Dry.Buffer == Device->RealOut.Buffer)
                    {
                        int idx = GetChannelIdxByName(&Device->RealOut, chans[c].channel);
                        if(idx != -1) voice->Direct.Params[c].Gains.Target[idx] = DryGain;
                    }
                    continue;
                }

                CalcAngleCoeffs(
                    (Device->Render_Mode==StereoPair) ? ScaleAzimuthFront(chans[c].angle, 3.0f)
                                                      : chans[c].angle,
                    chans[c].elevation, Spread, coeffs
                );

                ComputePanGains(&Device->Dry, coeffs, DryGain,
                                voice->Direct.Params[c].Gains.Target);
                for(i = 0;i < NumSends;i++)
                {
                    const ALeffectslot *Slot = SendSlots[i];
                    if(Slot)
                        ComputePanningGainsBF(Slot->ChanMap, Slot->NumChannels,
                            coeffs, WetGain[i], voice->Send[i].Params[c].Gains.Target
                        );
                }
            }
        }
    }

    {
        ALfloat hfScale = props->Direct.HFReference / Frequency;
        ALfloat lfScale = props->Direct.LFReference / Frequency;
        ALfloat gainHF = maxf(DryGainHF, 0.001f); /* Limit -60dB */
        ALfloat gainLF = maxf(DryGainLF, 0.001f);

        voice->Direct.FilterType = AF_None;
        if(gainHF != 1.0f) voice->Direct.FilterType |= AF_LowPass;
        if(gainLF != 1.0f) voice->Direct.FilterType |= AF_HighPass;
        BiquadFilter_setParams(
            &voice->Direct.Params[0].LowPass, BiquadType::HighShelf,
            gainHF, hfScale, calc_rcpQ_from_slope(gainHF, 1.0f)
        );
        BiquadFilter_setParams(
            &voice->Direct.Params[0].HighPass, BiquadType::LowShelf,
            gainLF, lfScale, calc_rcpQ_from_slope(gainLF, 1.0f)
        );
        for(c = 1;c < num_channels;c++)
        {
            BiquadFilter_copyParams(&voice->Direct.Params[c].LowPass,
                                    &voice->Direct.Params[0].LowPass);
            BiquadFilter_copyParams(&voice->Direct.Params[c].HighPass,
                                    &voice->Direct.Params[0].HighPass);
        }
    }
    for(i = 0;i < NumSends;i++)
    {
        ALfloat hfScale = props->Send[i].HFReference / Frequency;
        ALfloat lfScale = props->Send[i].LFReference / Frequency;
        ALfloat gainHF = maxf(WetGainHF[i], 0.001f);
        ALfloat gainLF = maxf(WetGainLF[i], 0.001f);

        voice->Send[i].FilterType = AF_None;
        if(gainHF != 1.0f) voice->Send[i].FilterType |= AF_LowPass;
        if(gainLF != 1.0f) voice->Send[i].FilterType |= AF_HighPass;
        BiquadFilter_setParams(
            &voice->Send[i].Params[0].LowPass, BiquadType::HighShelf,
            gainHF, hfScale, calc_rcpQ_from_slope(gainHF, 1.0f)
        );
        BiquadFilter_setParams(
            &voice->Send[i].Params[0].HighPass, BiquadType::LowShelf,
            gainLF, lfScale, calc_rcpQ_from_slope(gainLF, 1.0f)
        );
        for(c = 1;c < num_channels;c++)
        {
            BiquadFilter_copyParams(&voice->Send[i].Params[c].LowPass,
                                    &voice->Send[i].Params[0].LowPass);
            BiquadFilter_copyParams(&voice->Send[i].Params[c].HighPass,
                                    &voice->Send[i].Params[0].HighPass);
        }
    }
}

void CalcNonAttnSourceParams(ALvoice *voice, const struct ALvoiceProps *props, const ALbuffer *ALBuffer, const ALCcontext *ALContext)
{
    const ALCdevice *Device = ALContext->Device;
    const ALlistener &Listener = ALContext->Listener;
    ALfloat DryGain, DryGainHF, DryGainLF;
    ALfloat WetGain[MAX_SENDS];
    ALfloat WetGainHF[MAX_SENDS];
    ALfloat WetGainLF[MAX_SENDS];
    ALeffectslot *SendSlots[MAX_SENDS];
    ALfloat Pitch;
    ALsizei i;

    voice->Direct.Buffer = Device->Dry.Buffer;
    voice->Direct.Channels = Device->Dry.NumChannels;
    for(i = 0;i < Device->NumAuxSends;i++)
    {
        SendSlots[i] = props->Send[i].Slot;
        if(!SendSlots[i] && i == 0)
            SendSlots[i] = ALContext->DefaultSlot.get();
        if(!SendSlots[i] || SendSlots[i]->Params.EffectType == AL_EFFECT_NULL)
        {
            SendSlots[i] = NULL;
            voice->Send[i].Buffer = NULL;
            voice->Send[i].Channels = 0;
        }
        else
        {
            voice->Send[i].Buffer = SendSlots[i]->WetBuffer;
            voice->Send[i].Channels = SendSlots[i]->NumChannels;
        }
    }

    /* Calculate the stepping value */
    Pitch = (ALfloat)ALBuffer->Frequency/(ALfloat)Device->Frequency * props->Pitch;
    if(Pitch > (ALfloat)MAX_PITCH)
        voice->Step = MAX_PITCH<<FRACTIONBITS;
    else
        voice->Step = maxi(fastf2i(Pitch * FRACTIONONE), 1);
    if(props->Resampler == BSinc24Resampler)
        BsincPrepare(voice->Step, &voice->ResampleState.bsinc, &bsinc24);
    else if(props->Resampler == BSinc12Resampler)
        BsincPrepare(voice->Step, &voice->ResampleState.bsinc, &bsinc12);
    voice->Resampler = SelectResampler(props->Resampler);

    /* Calculate gains */
    DryGain  = clampf(props->Gain, props->MinGain, props->MaxGain);
    DryGain *= props->Direct.Gain * Listener.Params.Gain;
    DryGain  = minf(DryGain, GAIN_MIX_MAX);
    DryGainHF = props->Direct.GainHF;
    DryGainLF = props->Direct.GainLF;
    for(i = 0;i < Device->NumAuxSends;i++)
    {
        WetGain[i]  = clampf(props->Gain, props->MinGain, props->MaxGain);
        WetGain[i] *= props->Send[i].Gain * Listener.Params.Gain;
        WetGain[i]  = minf(WetGain[i], GAIN_MIX_MAX);
        WetGainHF[i] = props->Send[i].GainHF;
        WetGainLF[i] = props->Send[i].GainLF;
    }

    CalcPanningAndFilters(voice, 0.0f, 0.0f, 0.0f, 0.0f, DryGain, DryGainHF, DryGainLF, WetGain,
                          WetGainLF, WetGainHF, SendSlots, ALBuffer, props, Listener, Device);
}

void CalcAttnSourceParams(ALvoice *voice, const struct ALvoiceProps *props, const ALbuffer *ALBuffer, const ALCcontext *ALContext)
{
    const ALCdevice *Device = ALContext->Device;
    const ALlistener &Listener = ALContext->Listener;
    const ALsizei NumSends = Device->NumAuxSends;
    aluVector Position, Velocity, Direction, SourceToListener;
    ALfloat Distance, ClampedDist, DopplerFactor;
    ALeffectslot *SendSlots[MAX_SENDS];
    ALfloat RoomRolloff[MAX_SENDS];
    ALfloat DecayDistance[MAX_SENDS];
    ALfloat DecayLFDistance[MAX_SENDS];
    ALfloat DecayHFDistance[MAX_SENDS];
    ALfloat DryGain, DryGainHF, DryGainLF;
    ALfloat WetGain[MAX_SENDS];
    ALfloat WetGainHF[MAX_SENDS];
    ALfloat WetGainLF[MAX_SENDS];
    bool directional;
    ALfloat ev, az;
    ALfloat spread;
    ALfloat Pitch;
    ALint i;

    /* Set mixing buffers and get send parameters. */
    voice->Direct.Buffer = Device->Dry.Buffer;
    voice->Direct.Channels = Device->Dry.NumChannels;
    for(i = 0;i < NumSends;i++)
    {
        SendSlots[i] = props->Send[i].Slot;
        if(!SendSlots[i] && i == 0)
            SendSlots[i] = ALContext->DefaultSlot.get();
        if(!SendSlots[i] || SendSlots[i]->Params.EffectType == AL_EFFECT_NULL)
        {
            SendSlots[i] = NULL;
            RoomRolloff[i] = 0.0f;
            DecayDistance[i] = 0.0f;
            DecayLFDistance[i] = 0.0f;
            DecayHFDistance[i] = 0.0f;
        }
        else if(SendSlots[i]->Params.AuxSendAuto)
        {
            RoomRolloff[i] = SendSlots[i]->Params.RoomRolloff + props->RoomRolloffFactor;
            /* Calculate the distances to where this effect's decay reaches
             * -60dB.
             */
            DecayDistance[i] = SendSlots[i]->Params.DecayTime *
                               Listener.Params.ReverbSpeedOfSound;
            DecayLFDistance[i] = DecayDistance[i] * SendSlots[i]->Params.DecayLFRatio;
            DecayHFDistance[i] = DecayDistance[i] * SendSlots[i]->Params.DecayHFRatio;
            if(SendSlots[i]->Params.DecayHFLimit)
            {
                ALfloat airAbsorption = SendSlots[i]->Params.AirAbsorptionGainHF;
                if(airAbsorption < 1.0f)
                {
                    /* Calculate the distance to where this effect's air
                     * absorption reaches -60dB, and limit the effect's HF
                     * decay distance (so it doesn't take any longer to decay
                     * than the air would allow).
                     */
                    ALfloat absorb_dist = log10f(REVERB_DECAY_GAIN) / log10f(airAbsorption);
                    DecayHFDistance[i] = minf(absorb_dist, DecayHFDistance[i]);
                }
            }
        }
        else
        {
            /* If the slot's auxiliary send auto is off, the data sent to the
             * effect slot is the same as the dry path, sans filter effects */
            RoomRolloff[i] = props->RolloffFactor;
            DecayDistance[i] = 0.0f;
            DecayLFDistance[i] = 0.0f;
            DecayHFDistance[i] = 0.0f;
        }

        if(!SendSlots[i])
        {
            voice->Send[i].Buffer = NULL;
            voice->Send[i].Channels = 0;
        }
        else
        {
            voice->Send[i].Buffer = SendSlots[i]->WetBuffer;
            voice->Send[i].Channels = SendSlots[i]->NumChannels;
        }
    }

    /* Transform source to listener space (convert to head relative) */
    aluVectorSet(&Position, props->Position[0], props->Position[1], props->Position[2], 1.0f);
    aluVectorSet(&Direction, props->Direction[0], props->Direction[1], props->Direction[2], 0.0f);
    aluVectorSet(&Velocity, props->Velocity[0], props->Velocity[1], props->Velocity[2], 0.0f);
    if(props->HeadRelative == AL_FALSE)
    {
        const aluMatrixf *Matrix = &Listener.Params.Matrix;
        /* Transform source vectors */
        Position = aluMatrixfVector(Matrix, &Position);
        Velocity = aluMatrixfVector(Matrix, &Velocity);
        Direction = aluMatrixfVector(Matrix, &Direction);
    }
    else
    {
        const aluVector *lvelocity = &Listener.Params.Velocity;
        /* Offset the source velocity to be relative of the listener velocity */
        Velocity.v[0] += lvelocity->v[0];
        Velocity.v[1] += lvelocity->v[1];
        Velocity.v[2] += lvelocity->v[2];
    }

    directional = aluNormalize(Direction.v) > 0.0f;
    SourceToListener.v[0] = -Position.v[0];
    SourceToListener.v[1] = -Position.v[1];
    SourceToListener.v[2] = -Position.v[2];
    SourceToListener.v[3] = 0.0f;
    Distance = aluNormalize(SourceToListener.v);

    /* Initial source gain */
    DryGain = props->Gain;
    DryGainHF = 1.0f;
    DryGainLF = 1.0f;
    for(i = 0;i < NumSends;i++)
    {
        WetGain[i] = props->Gain;
        WetGainHF[i] = 1.0f;
        WetGainLF[i] = 1.0f;
    }

    /* Calculate distance attenuation */
    ClampedDist = Distance;

    switch(Listener.Params.SourceDistanceModel ?
           props->mDistanceModel : Listener.Params.mDistanceModel)
    {
        case DistanceModel::InverseClamped:
            ClampedDist = clampf(ClampedDist, props->RefDistance, props->MaxDistance);
            if(props->MaxDistance < props->RefDistance)
                break;
            /*fall-through*/
        case DistanceModel::Inverse:
            if(!(props->RefDistance > 0.0f))
                ClampedDist = props->RefDistance;
            else
            {
                ALfloat dist = lerp(props->RefDistance, ClampedDist, props->RolloffFactor);
                if(dist > 0.0f) DryGain *= props->RefDistance / dist;
                for(i = 0;i < NumSends;i++)
                {
                    dist = lerp(props->RefDistance, ClampedDist, RoomRolloff[i]);
                    if(dist > 0.0f) WetGain[i] *= props->RefDistance / dist;
                }
            }
            break;

        case DistanceModel::LinearClamped:
            ClampedDist = clampf(ClampedDist, props->RefDistance, props->MaxDistance);
            if(props->MaxDistance < props->RefDistance)
                break;
            /*fall-through*/
        case DistanceModel::Linear:
            if(!(props->MaxDistance != props->RefDistance))
                ClampedDist = props->RefDistance;
            else
            {
                ALfloat attn = props->RolloffFactor * (ClampedDist-props->RefDistance) /
                               (props->MaxDistance-props->RefDistance);
                DryGain *= maxf(1.0f - attn, 0.0f);
                for(i = 0;i < NumSends;i++)
                {
                    attn = RoomRolloff[i] * (ClampedDist-props->RefDistance) /
                           (props->MaxDistance-props->RefDistance);
                    WetGain[i] *= maxf(1.0f - attn, 0.0f);
                }
            }
            break;

        case DistanceModel::ExponentClamped:
            ClampedDist = clampf(ClampedDist, props->RefDistance, props->MaxDistance);
            if(props->MaxDistance < props->RefDistance)
                break;
            /*fall-through*/
        case DistanceModel::Exponent:
            if(!(ClampedDist > 0.0f && props->RefDistance > 0.0f))
                ClampedDist = props->RefDistance;
            else
            {
                DryGain *= powf(ClampedDist/props->RefDistance, -props->RolloffFactor);
                for(i = 0;i < NumSends;i++)
                    WetGain[i] *= powf(ClampedDist/props->RefDistance, -RoomRolloff[i]);
            }
            break;

        case DistanceModel::Disable:
            ClampedDist = props->RefDistance;
            break;
    }

    /* Calculate directional soundcones */
    if(directional && props->InnerAngle < 360.0f)
    {
        ALfloat ConeVolume;
        ALfloat ConeHF;
        ALfloat Angle;

        Angle = acosf(aluDotproduct(&Direction, &SourceToListener));
        Angle = RAD2DEG(Angle * ConeScale * 2.0f);
        if(!(Angle > props->InnerAngle))
        {
            ConeVolume = 1.0f;
            ConeHF = 1.0f;
        }
        else if(Angle < props->OuterAngle)
        {
            ALfloat scale = (            Angle-props->InnerAngle) /
                            (props->OuterAngle-props->InnerAngle);
            ConeVolume = lerp(1.0f, props->OuterGain, scale);
            ConeHF = lerp(1.0f, props->OuterGainHF, scale);
        }
        else
        {
            ConeVolume = props->OuterGain;
            ConeHF = props->OuterGainHF;
        }

        DryGain *= ConeVolume;
        if(props->DryGainHFAuto)
            DryGainHF *= ConeHF;
        if(props->WetGainAuto)
        {
            for(i = 0;i < NumSends;i++)
                WetGain[i] *= ConeVolume;
        }
        if(props->WetGainHFAuto)
        {
            for(i = 0;i < NumSends;i++)
                WetGainHF[i] *= ConeHF;
        }
    }

    /* Apply gain and frequency filters */
    DryGain  = clampf(DryGain, props->MinGain, props->MaxGain);
    DryGain  = minf(DryGain*props->Direct.Gain*Listener.Params.Gain, GAIN_MIX_MAX);
    DryGainHF *= props->Direct.GainHF;
    DryGainLF *= props->Direct.GainLF;
    for(i = 0;i < NumSends;i++)
    {
        WetGain[i]  = clampf(WetGain[i], props->MinGain, props->MaxGain);
        WetGain[i]  = minf(WetGain[i]*props->Send[i].Gain*Listener.Params.Gain, GAIN_MIX_MAX);
        WetGainHF[i] *= props->Send[i].GainHF;
        WetGainLF[i] *= props->Send[i].GainLF;
    }

    /* Distance-based air absorption and initial send decay. */
    if(ClampedDist > props->RefDistance && props->RolloffFactor > 0.0f)
    {
        ALfloat meters_base = (ClampedDist-props->RefDistance) * props->RolloffFactor *
                              Listener.Params.MetersPerUnit;
        if(props->AirAbsorptionFactor > 0.0f)
        {
            ALfloat hfattn = powf(AIRABSORBGAINHF, meters_base * props->AirAbsorptionFactor);
            DryGainHF *= hfattn;
            for(i = 0;i < NumSends;i++)
                WetGainHF[i] *= hfattn;
        }

        if(props->WetGainAuto)
        {
            /* Apply a decay-time transformation to the wet path, based on the
             * source distance in meters. The initial decay of the reverb
             * effect is calculated and applied to the wet path.
             */
            for(i = 0;i < NumSends;i++)
            {
                ALfloat gain, gainhf, gainlf;

                if(!(DecayDistance[i] > 0.0f))
                    continue;

                gain = powf(REVERB_DECAY_GAIN, meters_base/DecayDistance[i]);
                WetGain[i] *= gain;
                /* Yes, the wet path's air absorption is applied with
                 * WetGainAuto on, rather than WetGainHFAuto.
                 */
                if(gain > 0.0f)
                {
                    gainhf = powf(REVERB_DECAY_GAIN, meters_base/DecayHFDistance[i]);
                    WetGainHF[i] *= minf(gainhf / gain, 1.0f);
                    gainlf = powf(REVERB_DECAY_GAIN, meters_base/DecayLFDistance[i]);
                    WetGainLF[i] *= minf(gainlf / gain, 1.0f);
                }
            }
        }
    }


    /* Initial source pitch */
    Pitch = props->Pitch;

    /* Calculate velocity-based doppler effect */
    DopplerFactor = props->DopplerFactor * Listener.Params.DopplerFactor;
    if(DopplerFactor > 0.0f)
    {
        const aluVector *lvelocity = &Listener.Params.Velocity;
        const ALfloat SpeedOfSound = Listener.Params.SpeedOfSound;
        ALfloat vss, vls;

        vss = aluDotproduct(&Velocity, &SourceToListener) * DopplerFactor;
        vls = aluDotproduct(lvelocity, &SourceToListener) * DopplerFactor;

        if(!(vls < SpeedOfSound))
        {
            /* Listener moving away from the source at the speed of sound.
             * Sound waves can't catch it.
             */
            Pitch = 0.0f;
        }
        else if(!(vss < SpeedOfSound))
        {
            /* Source moving toward the listener at the speed of sound. Sound
             * waves bunch up to extreme frequencies.
             */
            Pitch = HUGE_VALF;
        }
        else
        {
            /* Source and listener movement is nominal. Calculate the proper
             * doppler shift.
             */
            Pitch *= (SpeedOfSound-vls) / (SpeedOfSound-vss);
        }
    }

    /* Adjust pitch based on the buffer and output frequencies, and calculate
     * fixed-point stepping value.
     */
    Pitch *= (ALfloat)ALBuffer->Frequency/(ALfloat)Device->Frequency;
    if(Pitch > (ALfloat)MAX_PITCH)
        voice->Step = MAX_PITCH<<FRACTIONBITS;
    else
        voice->Step = maxi(fastf2i(Pitch * FRACTIONONE), 1);
    if(props->Resampler == BSinc24Resampler)
        BsincPrepare(voice->Step, &voice->ResampleState.bsinc, &bsinc24);
    else if(props->Resampler == BSinc12Resampler)
        BsincPrepare(voice->Step, &voice->ResampleState.bsinc, &bsinc12);
    voice->Resampler = SelectResampler(props->Resampler);

    if(Distance > 0.0f)
    {
        /* Clamp Y, in case rounding errors caused it to end up outside of
         * -1...+1.
         */
        ev = asinf(clampf(-SourceToListener.v[1], -1.0f, 1.0f));
        /* Double negation on Z cancels out; negate once for changing source-
         * to-listener to listener-to-source, and again for right-handed coords
         * with -Z in front.
         */
        az = atan2f(-SourceToListener.v[0], SourceToListener.v[2]*ZScale);
    }
    else
        ev = az = 0.0f;

    if(props->Radius > Distance)
        spread = F_TAU - Distance/props->Radius*F_PI;
    else if(Distance > 0.0f)
        spread = asinf(props->Radius / Distance) * 2.0f;
    else
        spread = 0.0f;

    CalcPanningAndFilters(voice, az, ev, Distance, spread, DryGain, DryGainHF, DryGainLF, WetGain,
                          WetGainLF, WetGainHF, SendSlots, ALBuffer, props, Listener, Device);
}

void CalcSourceParams(ALvoice *voice, ALCcontext *context, bool force)
{
    ALvoiceProps *props{voice->Update.exchange(nullptr, std::memory_order_acq_rel)};
    if(!props && !force) return;

    if(props)
    {
        memcpy(voice->Props, props,
            FAM_SIZE(struct ALvoiceProps, Send, context->Device->NumAuxSends)
        );

        AtomicReplaceHead(context->FreeVoiceProps, props);
    }
    props = voice->Props;

    ALbufferlistitem *BufferListItem{voice->current_buffer.load(std::memory_order_relaxed)};
    while(BufferListItem)
    {
        auto buffers_end = BufferListItem->buffers+BufferListItem->num_buffers;
        auto buffer = std::find_if(BufferListItem->buffers, buffers_end,
            [](const ALbuffer *buffer) noexcept -> bool
            { return buffer != nullptr; }
        );
        if(LIKELY(buffer != buffers_end))
        {
            if(props->SpatializeMode == SpatializeOn ||
               (props->SpatializeMode == SpatializeAuto && (*buffer)->FmtChannels == FmtMono))
                CalcAttnSourceParams(voice, props, *buffer, context);
            else
                CalcNonAttnSourceParams(voice, props, *buffer, context);
            break;
        }
        BufferListItem = BufferListItem->next.load(std::memory_order_acquire);
    }
}


void ProcessParamUpdates(ALCcontext *ctx, const ALeffectslotArray *slots)
{
    IncrementRef(&ctx->UpdateCount);
    if(LIKELY(!ctx->HoldUpdates.load(std::memory_order_acquire)))
    {
        bool cforce = CalcContextParams(ctx);
        bool force = CalcListenerParams(ctx) | cforce;
        std::for_each(slots->slot, slots->slot+slots->count,
            [ctx,cforce,&force](ALeffectslot *slot) -> void
            { force |= CalcEffectSlotParams(slot, ctx, cforce); }
        );

        std::for_each(ctx->Voices, ctx->Voices+ctx->VoiceCount,
            [ctx,force](ALvoice *voice) -> void
            {
                ALsource *source{voice->Source.load(std::memory_order_acquire)};
                if(source) CalcSourceParams(voice, ctx, force);
            }
        );
    }
    IncrementRef(&ctx->UpdateCount);
}

void ProcessContext(ALCcontext *ctx, ALsizei SamplesToDo)
{
    const ALeffectslotArray *auxslots{ctx->ActiveAuxSlots.load(std::memory_order_acquire)};

    /* Process pending propery updates for objects on the context. */
    ProcessParamUpdates(ctx, auxslots);

    /* Clear auxiliary effect slot mixing buffers. */
    std::for_each(auxslots->slot, auxslots->slot+auxslots->count,
        [SamplesToDo](ALeffectslot *slot) -> void
        {
            std::for_each(slot->WetBuffer, slot->WetBuffer+slot->NumChannels,
                [SamplesToDo](ALfloat *buffer) -> void
                { std::fill_n(buffer, SamplesToDo, 0.0f); }
            );
        }
    );

    /* Process voices that have a playing source. */
    std::for_each(ctx->Voices, ctx->Voices+ctx->VoiceCount,
        [SamplesToDo,ctx](ALvoice *voice) -> void
        {
            ALsource *source{voice->Source.load(std::memory_order_acquire)};
            if(!source) return;
            if(!voice->Playing.load(std::memory_order_relaxed) || voice->Step < 1)
                return;

            if(!MixSource(voice, source->id, ctx, SamplesToDo))
            {
                voice->Source.store(nullptr, std::memory_order_relaxed);
                voice->Playing.store(false, std::memory_order_release);
                SendSourceStoppedEvent(ctx, source->id);
            }
        }
    );

    /* Process effects. */
    std::for_each(auxslots->slot, auxslots->slot+auxslots->count,
        [SamplesToDo](const ALeffectslot *slot) -> void
        {
            EffectState *state{slot->Params.mEffectState};
            state->process(SamplesToDo, slot->WetBuffer, state->mOutBuffer,
                           state->mOutChannels);
        }
    );
}


void ApplyStablizer(FrontStablizer *Stablizer, ALfloat (*RESTRICT Buffer)[BUFFERSIZE],
                    int lidx, int ridx, int cidx, ALsizei SamplesToDo, ALsizei NumChannels)
{
    ALfloat (*RESTRICT lsplit)[BUFFERSIZE] = Stablizer->LSplit;
    ALfloat (*RESTRICT rsplit)[BUFFERSIZE] = Stablizer->RSplit;
    ALsizei i;

    /* Apply an all-pass to all channels, except the front-left and front-
     * right, so they maintain the same relative phase.
     */
    for(i = 0;i < NumChannels;i++)
    {
        if(i == lidx || i == ridx)
            continue;
        splitterap_process(&Stablizer->APFilter[i], Buffer[i], SamplesToDo);
    }

    bandsplit_process(&Stablizer->LFilter, lsplit[1], lsplit[0], Buffer[lidx], SamplesToDo);
    bandsplit_process(&Stablizer->RFilter, rsplit[1], rsplit[0], Buffer[ridx], SamplesToDo);

    for(i = 0;i < SamplesToDo;i++)
    {
        ALfloat lfsum, hfsum;
        ALfloat m, s, c;

        lfsum = lsplit[0][i] + rsplit[0][i];
        hfsum = lsplit[1][i] + rsplit[1][i];
        s = lsplit[0][i] + lsplit[1][i] - rsplit[0][i] - rsplit[1][i];

        /* This pans the separate low- and high-frequency sums between being on
         * the center channel and the left/right channels. The low-frequency
         * sum is 1/3rd toward center (2/3rds on left/right) and the high-
         * frequency sum is 1/4th toward center (3/4ths on left/right). These
         * values can be tweaked.
         */
        m = lfsum*cosf(1.0f/3.0f * F_PI_2) + hfsum*cosf(1.0f/4.0f * F_PI_2);
        c = lfsum*sinf(1.0f/3.0f * F_PI_2) + hfsum*sinf(1.0f/4.0f * F_PI_2);

        /* The generated center channel signal adds to the existing signal,
         * while the modified left and right channels replace.
         */
        Buffer[lidx][i] = (m + s) * 0.5f;
        Buffer[ridx][i] = (m - s) * 0.5f;
        Buffer[cidx][i] += c * 0.5f;
    }
}

void ApplyDistanceComp(ALfloat (*RESTRICT Samples)[BUFFERSIZE], const DistanceComp &distcomp,
                       ALfloat *RESTRICT Values, ALsizei SamplesToDo, ALsizei numchans)
{
    for(ALsizei c{0};c < numchans;c++)
    {
        ALfloat *RESTRICT inout = Samples[c];
        const ALfloat gain = distcomp[c].Gain;
        const ALsizei base = distcomp[c].Length;
        ALfloat *RESTRICT distbuf = distcomp[c].Buffer;

        if(base == 0)
        {
            if(gain < 1.0f)
                std::for_each(inout, inout+SamplesToDo,
                    [gain](ALfloat &in) noexcept -> void
                    { in *= gain; }
                );
            continue;
        }

        if(LIKELY(SamplesToDo >= base))
        {
            auto out = std::copy_n(distbuf, base, Values);
            std::copy_n(inout, SamplesToDo-base, out);
            std::copy_n(inout+SamplesToDo-base, base, distbuf);
        }
        else
        {
            std::copy_n(distbuf, SamplesToDo, Values);
            auto out = std::copy(distbuf+SamplesToDo, distbuf+base, distbuf);
            std::copy_n(inout, SamplesToDo, out);
        }
        std::transform<ALfloat*RESTRICT>(Values, Values+SamplesToDo, inout,
            [gain](ALfloat in) noexcept -> ALfloat
            { return in * gain; }
        );
    }
}

void ApplyDither(ALfloat (*RESTRICT Samples)[BUFFERSIZE], ALuint *dither_seed,
                 const ALfloat quant_scale, const ALsizei SamplesToDo, const ALsizei numchans)
{
    ASSUME(numchans > 0);

    /* Dithering. Generate whitenoise (uniform distribution of random values
     * between -1 and +1) and add it to the sample values, after scaling up to
     * the desired quantization depth amd before rounding.
     */
    const ALfloat invscale = 1.0f / quant_scale;
    ALuint seed = *dither_seed;
    auto dither_channel = [&seed,invscale,quant_scale,SamplesToDo](ALfloat *buffer) -> void
    {
        ASSUME(SamplesToDo > 0);
        std::transform(buffer, buffer+SamplesToDo, buffer,
            [&seed,invscale,quant_scale](ALfloat sample) noexcept -> ALfloat
            {
                ALfloat val = sample * quant_scale;
                ALuint rng0 = dither_rng(&seed);
                ALuint rng1 = dither_rng(&seed);
                val += (ALfloat)(rng0*(1.0/UINT_MAX) - rng1*(1.0/UINT_MAX));
                return fast_roundf(val) * invscale;
            }
        );
    };
    std::for_each(Samples, Samples+numchans, dither_channel);
    *dither_seed = seed;
}


/* Base template left undefined. Should be marked =delete, but Clang 3.8.1
 * chokes on that given the inline specializations.
 */
template<typename T>
inline T SampleConv(ALfloat) noexcept;

template<> inline ALfloat SampleConv(ALfloat val) noexcept
{ return val; }
template<> inline ALint SampleConv(ALfloat val) noexcept
{
    /* Floats have a 23-bit mantissa. There is an implied 1 bit in the mantissa
     * along with the sign bit, giving 25 bits total, so [-16777216, +16777216]
     * is the max value a normalized float can be scaled to before losing
     * precision.
     */
    return fastf2i(clampf(val*16777216.0f, -16777216.0f, 16777215.0f))<<7;
}
template<> inline ALshort SampleConv(ALfloat val) noexcept
{ return fastf2i(clampf(val*32768.0f, -32768.0f, 32767.0f)); }
template<> inline ALbyte SampleConv(ALfloat val) noexcept
{ return fastf2i(clampf(val*128.0f, -128.0f, 127.0f)); }

/* Define unsigned output variations. */
template<> inline ALuint SampleConv(ALfloat val) noexcept
{ return SampleConv<ALint>(val) + 2147483648u; }
template<> inline ALushort SampleConv(ALfloat val) noexcept
{ return SampleConv<ALshort>(val) + 32768; }
template<> inline ALubyte SampleConv(ALfloat val) noexcept
{ return SampleConv<ALbyte>(val) + 128; }

template<DevFmtType T>
void Write(const ALfloat (*RESTRICT InBuffer)[BUFFERSIZE], ALvoid *OutBuffer,
           ALsizei Offset, ALsizei SamplesToDo, ALsizei numchans)
{
    using SampleType = typename DevFmtTypeTraits<T>::Type;

    ASSUME(numchans > 0);
    SampleType *outbase = static_cast<SampleType*>(OutBuffer) + Offset*numchans;
    auto conv_channel = [&outbase,SamplesToDo,numchans](const ALfloat *inbuf) -> void
    {
        ASSUME(SamplesToDo > 0);
        SampleType *out{outbase++};
        std::for_each<const ALfloat*RESTRICT>(inbuf, inbuf+SamplesToDo,
            [numchans,&out](const ALfloat s) noexcept -> void
            {
                *out = SampleConv<SampleType>(s);
                out += numchans;
            }
        );
    };
    std::for_each(InBuffer, InBuffer+numchans, conv_channel);
}

} // namespace

void aluMixData(ALCdevice *device, ALvoid *OutBuffer, ALsizei NumSamples)
{
    FPUCtl mixer_mode{};
    for(ALsizei SamplesDone{0};SamplesDone < NumSamples;)
    {
        const ALsizei SamplesToDo{mini(NumSamples-SamplesDone, BUFFERSIZE)};

        /* Clear main mixing buffers. */
        std::for_each(device->MixBuffer.begin(), device->MixBuffer.end(),
            [SamplesToDo](std::array<ALfloat,BUFFERSIZE> &buffer) -> void
            { std::fill_n(buffer.begin(), SamplesToDo, 0.0f); }
        );

        /* Increment the mix count at the start (lsb should now be 1). */
        IncrementRef(&device->MixCount);

        /* For each context on this device, process and mix its sources and
         * effects.
         */
        ALCcontext *ctx{device->ContextList.load(std::memory_order_acquire)};
        while(ctx)
        {
            ProcessContext(ctx, SamplesToDo);

            ctx = ctx->next.load(std::memory_order_relaxed);
        }

        /* Increment the clock time. Every second's worth of samples is
         * converted and added to clock base so that large sample counts don't
         * overflow during conversion. This also guarantees a stable
         * conversion.
         */
        device->SamplesDone += SamplesToDo;
        device->ClockBase += std::chrono::seconds{device->SamplesDone / device->Frequency};
        device->SamplesDone %= device->Frequency;

        /* Increment the mix count at the end (lsb should now be 0). */
        IncrementRef(&device->MixCount);

        /* Apply any needed post-process for finalizing the Dry mix to the
         * RealOut (Ambisonic decode, UHJ encode, etc).
         */
        if(LIKELY(device->PostProcess))
            device->PostProcess(device, SamplesToDo);

        /* Apply front image stablization for surround sound, if applicable. */
        if(device->Stablizer)
        {
            int lidx = GetChannelIdxByName(&device->RealOut, FrontLeft);
            int ridx = GetChannelIdxByName(&device->RealOut, FrontRight);
            int cidx = GetChannelIdxByName(&device->RealOut, FrontCenter);
            assert(lidx >= 0 && ridx >= 0 && cidx >= 0);

            ApplyStablizer(device->Stablizer.get(), device->RealOut.Buffer, lidx, ridx, cidx,
                           SamplesToDo, device->RealOut.NumChannels);
        }

        /* Apply delays and attenuation for mismatched speaker distances. */
        ApplyDistanceComp(device->RealOut.Buffer, device->ChannelDelay, device->TempBuffer[0],
                          SamplesToDo, device->RealOut.NumChannels);

        /* Apply compression, limiting final sample amplitude, if desired. */
        if(device->Limiter)
            ApplyCompression(device->Limiter.get(), SamplesToDo, device->RealOut.Buffer);

        /* Apply dithering. The compressor should have left enough headroom for
         * the dither noise to not saturate.
         */
        if(device->DitherDepth > 0.0f)
            ApplyDither(device->RealOut.Buffer, &device->DitherSeed, device->DitherDepth,
                        SamplesToDo, device->RealOut.NumChannels);

        if(LIKELY(OutBuffer))
        {
            ALfloat (*Buffer)[BUFFERSIZE] = device->RealOut.Buffer;
            ALsizei Channels = device->RealOut.NumChannels;

            /* Finally, interleave and convert samples, writing to the device's
             * output buffer.
             */
            switch(device->FmtType)
            {
#define HANDLE_WRITE(T) case T:                                            \
    Write<T>(Buffer, OutBuffer, SamplesDone, SamplesToDo, Channels); break;
                HANDLE_WRITE(DevFmtByte)
                HANDLE_WRITE(DevFmtUByte)
                HANDLE_WRITE(DevFmtShort)
                HANDLE_WRITE(DevFmtUShort)
                HANDLE_WRITE(DevFmtInt)
                HANDLE_WRITE(DevFmtUInt)
                HANDLE_WRITE(DevFmtFloat)
#undef HANDLE_WRITE
            }
        }

        SamplesDone += SamplesToDo;
    }
}


void aluHandleDisconnect(ALCdevice *device, const char *msg, ...)
{
    if(!device->Connected.exchange(AL_FALSE, std::memory_order_acq_rel))
        return;

    AsyncEvent evt = ASYNC_EVENT(EventType_Disconnected);
    evt.u.user.type = AL_EVENT_TYPE_DISCONNECTED_SOFT;
    evt.u.user.id = 0;
    evt.u.user.param = 0;

    va_list args;
    va_start(args, msg);
    int msglen{vsnprintf(evt.u.user.msg, sizeof(evt.u.user.msg), msg, args)};
    va_end(args);

    if(msglen < 0 || (size_t)msglen >= sizeof(evt.u.user.msg))
        evt.u.user.msg[sizeof(evt.u.user.msg)-1] = 0;

    ALCcontext *ctx{device->ContextList.load()};
    while(ctx)
    {
        ALbitfieldSOFT enabledevt = ctx->EnabledEvts.load(std::memory_order_acquire);
        if((enabledevt&EventType_Disconnected) &&
           ll_ringbuffer_write(ctx->AsyncEvents, &evt, 1) == 1)
            alsem_post(&ctx->EventSem);

        std::for_each(ctx->Voices, ctx->Voices+ctx->VoiceCount,
            [ctx](ALvoice *voice) -> void
            {
                ALsource *source{voice->Source.exchange(nullptr, std::memory_order_relaxed)};
                if(source && voice->Playing.load(std::memory_order_relaxed))
                {
                    /* If the source's voice was playing, it's now effectively
                     * stopped (the source state will be updated the next time
                     * it's checked).
                    */
                    SendSourceStoppedEvent(ctx, source->id);
                }
                voice->Playing.store(false, std::memory_order_release);
            }
        );

        ctx = ctx->next.load(std::memory_order_relaxed);
    }
}
