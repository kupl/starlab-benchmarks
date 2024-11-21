/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.
Copyright (C) 2005 Stuart Dalton (badcdev@gmail.com)

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/

#ifndef _SND_CODEC_H_
#define _SND_CODEC_H_

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

typedef struct snd_info_s
{
	int rate;
	int width;
	int channels;
	int samples;
	int size;
	int dataofs;
} snd_info_t;

typedef struct snd_codec_s snd_codec_t;

typedef struct snd_stream_s
{
	snd_codec_t *codec;
	fileHandle_t file;
	snd_info_t info;
	int length;
	int pos;
	void *ptr;
} snd_stream_t;

// Codec functions
typedef void *(*CODEC_LOAD)(const char *filename, snd_info_t *info);
typedef snd_stream_t *(*CODEC_OPEN)(const char *filename);
typedef int (*CODEC_READ)(snd_stream_t *stream, int bytes, void *buffer);
typedef void (*CODEC_CLOSE)(snd_stream_t *stream);

// Codec data structure
struct snd_codec_s
{
	char *ext;
	CODEC_LOAD load;
	CODEC_OPEN open;
	CODEC_READ read;
	CODEC_CLOSE close;
	snd_codec_t *next;
};

// Codec management
void S_CodecInit( void );
void S_CodecShutdown( void );
void S_CodecRegister(snd_codec_t *codec);
void *S_CodecLoad(const char *filename, snd_info_t *info);
snd_stream_t *S_CodecOpenStream(const char *filename);
void S_CodecCloseStream(snd_stream_t *stream);
int S_CodecReadStream(snd_stream_t *stream, int bytes, void *buffer);

// Util functions (used by codecs)
snd_stream_t *S_CodecUtilOpen(const char *filename, snd_codec_t *codec);
void S_CodecUtilClose(snd_stream_t **stream);

// WAV Codec
extern snd_codec_t wav_codec;
void *S_WAV_CodecLoad(const char *filename, snd_info_t *info);
snd_stream_t *S_WAV_CodecOpenStream(const char *filename);
void S_WAV_CodecCloseStream(snd_stream_t *stream);
int S_WAV_CodecReadStream(snd_stream_t *stream, int bytes, void *buffer);

// MP3 codec
#ifdef USE_CODEC_MP3
extern snd_codec_t mp3_codec;
void *S_MP3_CodecLoad(const char *filename, snd_info_t *info);
snd_stream_t *S_MP3_CodecOpenStream(const char *filename);
void S_MP3_CodecCloseStream(snd_stream_t *stream);
int S_MP3_CodecReadStream(snd_stream_t *stream, int bytes, void *buffer);
#endif // USE_CODEC_MP3

// Ogg Vorbis codec
#ifdef USE_CODEC_VORBIS
extern snd_codec_t ogg_codec;
void *S_OGG_CodecLoad(const char *filename, snd_info_t *info);
snd_stream_t *S_OGG_CodecOpenStream(const char *filename);
void S_OGG_CodecCloseStream(snd_stream_t *stream);
int S_OGG_CodecReadStream(snd_stream_t *stream, int bytes, void *buffer);
#endif // USE_CODEC_VORBIS

// Ogg Opus codec
#ifdef USE_CODEC_OPUS
extern snd_codec_t opus_codec;
void *S_OggOpus_CodecLoad(const char *filename, snd_info_t *info);
snd_stream_t *S_OggOpus_CodecOpenStream(const char *filename);
void S_OggOpus_CodecCloseStream(snd_stream_t *stream);
int S_OggOpus_CodecReadStream(snd_stream_t *stream, int bytes, void *buffer);
#endif // USE_CODEC_OPUS

#endif // !_SND_CODEC_H_
