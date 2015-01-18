/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
//#include <sys/shm.h>
#include <sys/wait.h>
#include <linux/soundcard.h>
#include <stdio.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "../../client/client.h"
#include "../../client/snd_loc.h"

int audio_fd;
int snd_inited;

cvar_t *sndbits;
cvar_t *sndspeed;
cvar_t *sndchannels;
cvar_t *snddevice;

static int tryrates[] = { 11025, 22051, 44100, 8000 };

// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;

// output mix interfaces
static SLObjectItf outputMixObject = NULL;

// buffer queue player interfaces
static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay;
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
static SLEffectSendItf bqPlayerEffectSend;
static SLVolumeItf bqPlayerVolume;


const size_t WAV_BUFFERS	=			128;
///const int#define	WAV_MASK				0x3F
const size_t WAV_BUFFER_SIZE =			0x0200;
//#define SECONDARY_BUFFER_SIZE	0x10000
size_t gSndBufSize;

int32_t completed = 0;
int32_t sent = 0;

static int32_t sample16;

// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
#if 0
    assert(bq == bqPlayerBufferQueue);
    assert(NULL == context);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    if (--nextCount > 0 && NULL != nextBuffer && 0 != nextSize) {
        SLresult result;
        // enqueue another buffer
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
#else

completed++;
    SLresult result;
byte *p = NULL;
    if (!dma.buffer) {
        return;
    }
    
    p = dma.buffer + (sent * WAV_BUFFER_SIZE);
        
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, p, WAV_BUFFER_SIZE);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        if (SL_RESULT_SUCCESS != result) Com_Printf("got result %d", result);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
        
        
        sent++;
        
        if (sent > WAV_BUFFERS) {
            sent = 0;
        }

#endif
}


qboolean SNDDMA_Init(void)
{
#if 0
	int rc;
    int fmt;
	int tmp;
    int i;
    char *s;
	struct audio_buf_info info;
	int caps;
	extern uid_t saved_euid;

	if (snd_inited)
		return;

	if (!snddevice) {
		sndbits = Cvar_Get("sndbits", "16", CVAR_ARCHIVE);
		sndspeed = Cvar_Get("sndspeed", "0", CVAR_ARCHIVE);
		sndchannels = Cvar_Get("sndchannels", "2", CVAR_ARCHIVE);
		snddevice = Cvar_Get("snddevice", "/dev/dsp", CVAR_ARCHIVE);
	}

// open /dev/dsp, confirm capability to mmap, and get size of dma buffer

	if (!audio_fd) {
		seteuid(saved_euid);

		audio_fd = open(snddevice->string, O_RDWR);

		seteuid(getuid());

		if (audio_fd < 0)
		{
			perror(snddevice->string);
			Com_Printf("Could not open %s\n", snddevice->string);
			return 0;
		}
	}

    rc = ioctl(audio_fd, SNDCTL_DSP_RESET, 0);
    if (rc < 0)
	{
		perror(snddevice->string);
		Com_Printf("Could not reset %s\n", snddevice->string);
		close(audio_fd);
		return 0;
	}

	if (ioctl(audio_fd, SNDCTL_DSP_GETCAPS, &caps)==-1)
	{
		perror(snddevice->string);
        Com_Printf("Sound driver too old\n");
		close(audio_fd);
		return 0;
	}

	if (!(caps & DSP_CAP_TRIGGER) || !(caps & DSP_CAP_MMAP))
	{
		Com_Printf("Sorry but your soundcard can't do this\n");
		close(audio_fd);
		return 0;
	}

    if (ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info)==-1)
    {   
        perror("GETOSPACE");
		Com_Printf("Um, can't do GETOSPACE?\n");
		close(audio_fd);
		return 0;
    }
    
// set sample bits & speed

    dma.samplebits = (int)sndbits->value;
	if (dma.samplebits != 16 && dma.samplebits != 8)
    {
        ioctl(audio_fd, SNDCTL_DSP_GETFMTS, &fmt);
        if (fmt & AFMT_S16_LE) dma.samplebits = 16;
        else if (fmt & AFMT_U8) dma.samplebits = 8;
    }

	dma.speed = (int)sndspeed->value;
	if (!dma.speed) {
        for (i=0 ; i<sizeof(tryrates)/4 ; i++)
            if (!ioctl(audio_fd, SNDCTL_DSP_SPEED, &tryrates[i])) break;
        dma.speed = tryrates[i];
    }

	dma.channels = (int)sndchannels->value;
	if (dma.channels < 1 || dma.channels > 2)
		dma.channels = 2;
	
	dma.samples = info.fragstotal * info.fragsize / (dma.samplebits/8);
	dma.submission_chunk = 1;

// memory map the dma buffer

	if (!dma.buffer)
		dma.buffer = (unsigned char *) mmap(NULL, info.fragstotal
			* info.fragsize, PROT_WRITE, MAP_FILE|MAP_SHARED, audio_fd, 0);
	if (!dma.buffer)
	{
		perror(snddevice->string);
		Com_Printf("Could not mmap %s\n", snddevice->string);
		close(audio_fd);
		return 0;
	}

	tmp = 0;
	if (dma.channels == 2)
		tmp = 1;
    rc = ioctl(audio_fd, SNDCTL_DSP_STEREO, &tmp);
    if (rc < 0)
    {
		perror(snddevice->string);
        Com_Printf("Could not set %s to stereo=%d", snddevice->string, dma.channels);
		close(audio_fd);
        return 0;
    }
	if (tmp)
		dma.channels = 2;
	else
		dma.channels = 1;

    rc = ioctl(audio_fd, SNDCTL_DSP_SPEED, &dma.speed);
    if (rc < 0)
    {
		perror(snddevice->string);
        Com_Printf("Could not set %s speed to %d", snddevice->string, dma.speed);
		close(audio_fd);
        return 0;
    }

    if (dma.samplebits == 16)
    {
        rc = AFMT_S16_LE;
        rc = ioctl(audio_fd, SNDCTL_DSP_SETFMT, &rc);
        if (rc < 0)
		{
			perror(snddevice->string);
			Com_Printf("Could not support 16-bit data.  Try 8-bit.\n");
			close(audio_fd);
			return 0;
		}
    }
    else if (dma.samplebits == 8)
    {
        rc = AFMT_U8;
        rc = ioctl(audio_fd, SNDCTL_DSP_SETFMT, &rc);
        if (rc < 0)
		{
			perror(snddevice->string);
			Com_Printf("Could not support 8-bit data.\n");
			close(audio_fd);
			return 0;
		}
    }
	else
	{
		perror(snddevice->string);
		Com_Printf("%d-bit sound not supported.", dma.samplebits);
		close(audio_fd);
		return 0;
	}

// toggle the trigger & start her up

    tmp = 0;
    rc  = ioctl(audio_fd, SNDCTL_DSP_SETTRIGGER, &tmp);
	if (rc < 0)
	{
		perror(snddevice->string);
		Com_Printf("Could not toggle.\n");
		close(audio_fd);
		return 0;
	}
    tmp = PCM_ENABLE_OUTPUT;
    rc = ioctl(audio_fd, SNDCTL_DSP_SETTRIGGER, &tmp);
	if (rc < 0)
	{
		perror(snddevice->string);
		Com_Printf("Could not toggle.\n");
		close(audio_fd);
		return 0;
	}

	dma.samplepos = 0;

	snd_inited = 1;
	return 1;
#else
//return 0;
if (dma.buffer) return 1;
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
    
    // create output mix, with environmental reverb specified as a non-required interface
 const SLInterfaceID ids[] = {SL_IID_VOLUME};
 const SLboolean req[] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 4};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1,
        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT /*SL_SPEAKER_FRONT_CENTER*/, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids2[] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req2[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk,
            2, ids2, req2);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the play interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the buffer queue interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
            &bqPlayerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // register callback on the buffer queue
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
/*
    // get the effect send interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND,
            &bqPlayerEffectSend);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
*/
#if 0   // mute/solo is not supported for sources that are known to be mono, as this is
    // get the mute/solo interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_MUTESOLO, &bqPlayerMuteSolo);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
#endif
/*
    // get the volume interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
*/
    // set the player's state to playing
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
    
    
    gSndBufSize = WAV_BUFFERS * WAV_BUFFER_SIZE;
    dma.samplebits = 16;
    dma.speed = 44100;
    dma.channels = 2;
    dma.samplepos = 0;
    dma.samples = gSndBufSize / (dma.samplebits/8);
    dma.submission_chunk = 512;
    dma.buffer = Z_Malloc(gSndBufSize); //  Hunk_AllocName(TOTAL_BUFFER_SIZE, (char*) "shmbuf");
    sample16 = (dma.samplebits/8) - 1;
    Com_Printf("Samples: %d", dma.samples);
    
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, dma.buffer, WAV_BUFFER_SIZE);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        if (SL_RESULT_SUCCESS != result) Com_Printf("got result %d", result);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
        sent++;
    
    return 1;

#endif
}

int SNDDMA_GetDMAPos(void)
{
#if 0
	struct count_info count;

	if (!snd_inited) return 0;

	if (ioctl(audio_fd, SNDCTL_DSP_GETOPTR, &count)==-1)
	{
		perror(snddevice->string);
		Com_Printf("Uh, sound dead.\n");
		close(audio_fd);
		snd_inited = 0;
		return 0;
	}
//	dma.samplepos = (count.bytes / (dma.samplebits / 8)) & (dma.samples-1);
//	fprintf(stderr, "%d    \r", count.ptr);
	dma.samplepos = count.ptr / (dma.samplebits / 8);

	return dma.samplepos;
#else
    // XXX: implement;
    
    int32_t s = sent * WAV_BUFFER_SIZE;
    
    s >>= sample16;

	s &= (dma.samples-1);
	
	return s;
    
#endif
}

void SNDDMA_Shutdown(void)
{
#if 0
	if (snd_inited)
	{
		close(audio_fd);
		snd_inited = 0;
	}
#endif
}

/*
==============
SNDDMA_Submit

Send sound to device if buffer isn't really the dma buffer
===============
*/
void SNDDMA_Submit(void)
{
    SLresult result;
    byte *p = NULL;
return;    
    if (!dma.buffer) {
        return;
    }
    


    Com_Printf("thingies %d %d %d %d %d ", sent, completed, sample16, (sent - completed) >> sample16, ((sent - completed) >> sample16) < 8);
   // while (((sent - completed) >> sample16) < 8) {
        //Com_Printf("dma.buffer %p %d %d %d %d %d", dma.buffer, dma.buffer[0], dma.buffer[1], dma.buffer[2], dma.buffer[3], dma.buffer[4]);
        p = dma.buffer + (sent * WAV_BUFFER_SIZE);
        sent++;
        
      //  if (paintedtime/256 <= sent) break;
        
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, p, WAV_BUFFER_SIZE);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        if (SL_RESULT_SUCCESS != result) Com_Printf("got result %d", result);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
        
        
//    }
        
        if (sent > WAV_BUFFERS) {
            sent = 0;
        }
    
}

void SNDDMA_BeginPainting (void)
{
}

