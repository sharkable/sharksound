//
//  AppleSound.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-01-15.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/apple/AppleSound.h"

using namespace SharkSound;
using std::string;

ALCcontext *AppleSound::context__ = NULL;
ALCdevice *AppleSound::device__ = NULL;

AppleSound::AppleSound(SoundController *sound_controller, const string &filename)
    : Sound(sound_controller),
      loop_source_id_(-1),
      loop_volume_(1.f) {
  if (device__ == NULL) {
    device__ = alcOpenDevice(NULL);
    if (device__) {
      context__ = alcCreateContext(device__, NULL);
      alcMakeContextCurrent(context__);
    }
  }

  ALvoid *audio_data;
  ALenum error = AL_NO_ERROR;
  ALenum format;
  ALsizei size;
  ALsizei freq;

  NSBundle *bundle = [NSBundle mainBundle];

  size_t dot_location = filename.find_last_of('.');
  NSString *ns_filename = [NSString stringWithUTF8String:filename.c_str()];
  NSString *pre_dot = [ns_filename substringToIndex:dot_location];
  NSString *post_dot = [ns_filename substringFromIndex:dot_location + 1];

  // get some audio data from a wave file
  CFURLRef file_url =
      (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:pre_dot ofType:post_dot]] retain];
  if (!file_url) {
    NSLog(@"ERRRRRRORRRR WITH FILE URL");
    return;  // TODO throw error... move work out of constructor.
  }

  audio_data = MyGetOpenALAudioData(file_url, &size, &format, &freq);

  CFRelease(file_url);

  // grab a buffer ID from openAL
  alGenBuffers(1, &buffer_id_);

  // load the awaiting data blob into the openAL buffer.
  alBufferData(buffer_id_, format, audio_data, size, freq);

  // add adsl;asdl;asd
  source_ids_.push_back(CreateNewSource());  // TODO check for error

  // clean up the buffer
  if (audio_data) {
    free(audio_data);
    audio_data = NULL;
  }

  if((error = alGetError()) != AL_NO_ERROR) {
    printf("error loading sound: %x\n", error);
  }
}

AppleSound::~AppleSound() {
  for (auto i = source_ids_.begin(); i != source_ids_.end(); i++) {
    alDeleteSources(1, &(*i));
  }
  alDeleteSources(1, &loop_source_id_);
  alDeleteBuffers(1, &buffer_id_);

  alcDestroyContext(context__);
  alcCloseDevice(device__);
}


#pragma mark - Audio

bool AppleSound::Play(float volume, float position) {
  if (!on_) {
    return false;
  }
  ALuint source_id = -1;
  for (auto i = source_ids_.begin(); i != source_ids_.end(); i++) {
    ALenum state;
    alGetSourcei(*i, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
      source_id = *i;
      break;
    }
  }
  if (source_id == -1) {
    source_id = CreateNewSource();
    if (source_id == -1) {
      return false;
    }
    source_ids_.push_back(source_id);
  }

  float gain = volume / 1.0f * 0.5f * global_volume_;
  alSourcef(source_id, AL_GAIN, gain);

  float sourcePosAL[] = {position, 0.0f, 0.0f};
  alSourcefv(source_id, AL_POSITION, sourcePosAL);

  alSourcePlay(source_id);

  ALenum error;
  if((error = alGetError()) != AL_NO_ERROR) {
    printf("error playing sound: %x\n", error);
    return false;
  }

  return true;
}

bool AppleSound::PlayLoop() {
  if (!on_) {
    return false;
  }
  if (loop_source_id_ == -1) {
    loop_source_id_ = CreateNewSource();
    if (loop_source_id_ == -1) {
      return false;
    }
    alSourcei(loop_source_id_, AL_LOOPING, AL_TRUE);
  }
  alSourcePlay(loop_source_id_);
  return true;
}

bool AppleSound::StopLoop() {
  if (loop_source_id_ == -1) {
    return false;
  }
  alSourceStop(loop_source_id_);
  return true;
}

void AppleSound::RewindLoop() {
  if (loop_source_id_ != -1) {
    alSourceRewind(loop_source_id_);
  }
}

void AppleSound::SetLoopVolume(float volume) {
  loop_volume_ = volume;
  if (loop_source_id_ != -1) {
    float gain = volume / 1.0f * 0.5f * global_volume_;
    alSourcef(loop_source_id_, AL_GAIN, gain);
  }
}

float AppleSound::LoopVolume() {
  return loop_volume_;
}

void AppleSound::SetLoopPosition(float position) {
  if (loop_source_id_ != -1) {
    float sourcePosAL[] = {position, 0.0f, 0.0f};
    alSourcefv(loop_source_id_, AL_POSITION, sourcePosAL);
  }
}

bool AppleSound::IsLoopPlaying() {
  if (loop_source_id_ == -1) {
    return false;
  }
  ALenum state;
  alGetSourcei(loop_source_id_, AL_SOURCE_STATE, &state);
  return (state == AL_PLAYING);
}


#pragma mark - private

ALuint AppleSound::CreateNewSource() {
  // grab a source ID from openAL
  ALuint source_id;
  alGenSources(1, &source_id);

  // attach the buffer to the source
  alSourcei(source_id, AL_BUFFER, buffer_id_);

  alSourcef(source_id, AL_PITCH, 1.0f);
  alSourcef(source_id, AL_GAIN,  1.0f);
  alSourcef(source_id, AL_MIN_GAIN,  0.0f);
  alSourcef(source_id, AL_MAX_GAIN,  1.0f);

  return source_id;
}
