/*
 * OpenAL example
 *
 * Copyright(C) Florian Fainelli <f.fainelli@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>


#ifdef LIBAUDIO
#include <audio/wave.h>
#endif

#include <al.h>
#include <alc.h>

ALint source_state;
ALCdevice *device;
ALCcontext *context;
ALuint buffer, source;
const ALCchar *devices;
int stoped = 0;

static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

#define TEST_ERROR(_msg)		\
error = alGetError();		\
if (error != AL_NO_ERROR) {	\
fprintf(stderr, _msg "\n");	\
return -1;		\
}

static inline ALenum to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}


int play(int argc, char *argv[], char* filePath)
{
	stoped = 0;
	ALboolean enumeration;
	ALvoid *data;
	char *bufferData;
	const ALCchar *defaultDeviceName = argv[1];
	printf("here here ");
	printf("%s", defaultDeviceName);
	ALsizei size, freq;
	ALenum format;
	int ret;
#ifdef LIBAUDIO
	WaveInfo *wave;
#endif
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;


	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	if (!defaultDeviceName)
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source);
	TEST_ERROR("source generation");

	alSourcef(source, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer);
	TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
	/* load data */
	wave = WaveOpenFileForReading(filePath);
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
		bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
#else
	//alutLoadWAVFile("/Users/margaretwm3/Dropbox/CSE167_Final_Project/BR_Pikachu.wav", &format, &data, &size, &freq, &loop);
	//alutLoadWAVFile("/Users/ruiqingqiu/Desktop/Qiu_Code/CSE167/CSE167 Final Project/BR_Pikachu.wav", &format, &data, &size, &freq, &loop);
	alutLoadWAVFile(filePath, &format, &data, &size, &freq, &loop);
	//alutLoadWAVFile("/Users/ruiqingqiu/Desktop/Qiu_Code/CSE167/CSE167 Final Project/Superheroes.wav", &format, &data, &size, &freq, &loop);
	//alutLoadWAVFile("/Users/margaretwm3/Dropbox/CSE167_Final_Project/Superheroes.wav", &format, &data, &size, &freq, &loop);
	TEST_ERROR("loading wav file");

	alBufferData(buffer, format, data, size, freq);
	TEST_ERROR("buffer copy");
#endif

	alSourcei(source, AL_BUFFER, buffer);
	TEST_ERROR("buffer binding");

	alSourcePlay(source);
	TEST_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	TEST_ERROR("source state get");

	//    while (source_state == AL_PLAYING) {
	//        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//        TEST_ERROR("source state get");
	//    }
	//    
	//    /* exit context */
	//    alDeleteSources(1, &source);
	//    alDeleteBuffers(1, &buffer);
	//    device = alcGetContextsDevice(context);
	//    alcMakeContextCurrent(NULL);
	//    alcDestroyContext(context);
	//    alcCloseDevice(device);

	return 0;
}

void stopPlaying() {
	//need to stop the applause only
	if (stoped == 0) {
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
		stoped = !stoped;
	}
}


ALint source_state1;
ALCdevice *device1;
ALCcontext *context1;
ALuint buffer1, source1;

int playApplause(int argc, char *argv[], char* filePath)
{
	ALboolean enumeration;
	ALvoid *data;
	char *bufferData;
	const ALCchar *defaultDeviceName = "alsa";
	printf("here here ");
	//printf("%s",  defaultDeviceName);
	ALsizei size, freq;
	ALenum format;
	int ret;
#ifdef LIBAUDIO
	WaveInfo *wave;
#endif
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;


	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	/*
	if (!defaultDeviceName)
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	*/
	device1 = alcOpenDevice(defaultDeviceName);
	if (!device1) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device1, ALC_DEVICE_SPECIFIER));

	alGetError();

	context1 = alcCreateContext(device1, NULL);
	if (!alcMakeContextCurrent(context1)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source1);
	TEST_ERROR("source generation");

	alSourcef(source1, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source1, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source1, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source1, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source1, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer1);
	TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
	/* load data */
	wave = WaveOpenFileForReading(filePath);
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
		bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
#else

	alutLoadWAVFile(filePath, &format, &data, &size, &freq, &loop);
	TEST_ERROR("loading wav file");

	alBufferData(buffer1, format, data, size, freq);
	TEST_ERROR("buffer copy");
#endif

	alSourcei(source1, AL_BUFFER, buffer1);
	TEST_ERROR("buffer binding");

	alSourcePlay(source1);
	TEST_ERROR("source playing");

	alGetSourcei(source1, AL_SOURCE_STATE, &source_state1);
	TEST_ERROR("source state get");

	//    while (source_state == AL_PLAYING) {
	//        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//        TEST_ERROR("source state get");
	//    }
	//

//        /* exit context */
//        alDeleteSources(1, &source1);
//        alDeleteBuffers(1, &buffer1);
//        device = alcGetContextsDevice(context1);
//        alcMakeContextCurrent(NULL);
//        alcDestroyContext(context1);
//        alcCloseDevice(device1);
//    
	return 0;
}

ALint source_state2;
ALCdevice *device2;
ALCcontext *context2;
ALuint buffer2, source2;
int playBoo(int argc, char *argv[], char* filePath)
{
	ALboolean enumeration;
	ALvoid *data;
	char *bufferData;
	const ALCchar *defaultDeviceName = "alsa";
	printf("here here ");
	printf("%s", defaultDeviceName);
	ALsizei size, freq;
	ALenum format;
	int ret;
#ifdef LIBAUDIO
	WaveInfo *wave;
#endif
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;


	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	/*
	 if (!defaultDeviceName)
	 defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	 */
	device2 = alcOpenDevice(defaultDeviceName);
	if (!device2) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device2, ALC_DEVICE_SPECIFIER));

	alGetError();

	context2 = alcCreateContext(device2, NULL);
	if (!alcMakeContextCurrent(context2)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source2);
	TEST_ERROR("source generation");

	alSourcef(source2, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source2, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source2, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source2, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source2, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer2);
	TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
	/* load data */
	wave = WaveOpenFileForReading(filePath);
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
		bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
#else
	alutLoadWAVFile(filePath, &format, &data, &size, &freq, &loop);
	TEST_ERROR("loading wav file");

	alBufferData(buffer2, format, data, size, freq);
	TEST_ERROR("buffer copy");
#endif

	alSourcei(source2, AL_BUFFER, buffer2);
	TEST_ERROR("buffer binding");

	alSourcePlay(source2);
	TEST_ERROR("source playing");

	alGetSourcei(source2, AL_SOURCE_STATE, &source_state2);
	TEST_ERROR("source state get");

	//    while (source_state == AL_PLAYING) {
	//        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//        TEST_ERROR("source state get");
	//    }
	//

	//        /* exit context */
	//        alDeleteSources(1, &source1);
	//        alDeleteBuffers(1, &buffer1);
	//        device = alcGetContextsDevice(context1);
	//        alcMakeContextCurrent(NULL);
	//        alcDestroyContext(context1);
	//        alcCloseDevice(device1);
	//    
	return 0;
}

ALint source_state3;
ALCdevice *device3;
ALCcontext *context3;
ALuint buffer3, source3;

int playTurnback(int argc, char *argv[], char* filePath) {
	ALboolean enumeration;
	ALvoid *data;
	char *bufferData;
	const ALCchar *defaultDeviceName = "sdl";
	printf("here here ");
	printf("%s", defaultDeviceName);
	ALsizei size, freq;
	ALenum format;
	int ret;
#ifdef LIBAUDIO
	WaveInfo *wave;
#endif
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	/*
	 if (!defaultDeviceName)
	 defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	 */
	device3 = alcOpenDevice(defaultDeviceName);
	if (!device3) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device3, ALC_DEVICE_SPECIFIER));

	alGetError();

	context3 = alcCreateContext(device3, NULL);
	if (!alcMakeContextCurrent(context3)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source3);
	TEST_ERROR("source generation");

	alSourcef(source3, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source3, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source3, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source3, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source3, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer3);
	TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
	/* load data */
	wave = WaveOpenFileForReading(filePath);
	if (!wave) {
		fprintf(stderr, "failed to read wave file\n");
		return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
		fprintf(stderr, "failed to seek wave file\n");
		return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
		perror("malloc");
		return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
		return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
		bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
#else
	alutLoadWAVFile(filePath, &format, &data, &size, &freq, &loop);
	TEST_ERROR("loading wav file");

	alBufferData(buffer3, format, data, size, freq);
	TEST_ERROR("buffer copy");
#endif

	alSourcei(source3, AL_BUFFER, buffer3);
	TEST_ERROR("buffer binding");

	alSourcePlay(source3);
	TEST_ERROR("source playing");

	alGetSourcei(source3, AL_SOURCE_STATE, &source_state3);
	TEST_ERROR("source state get");

	//    while (source_state == AL_PLAYING) {
	//        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//        TEST_ERROR("source state get");
	//    }
	//

	//        /* exit context */
	//        alDeleteSources(1, &source1);
	//        alDeleteBuffers(1, &buffer1);
	//        device = alcGetContextsDevice(context1);
	//        alcMakeContextCurrent(NULL);
	//        alcDestroyContext(context1);
	//        alcCloseDevice(device1);
	//
	return 0;


}

