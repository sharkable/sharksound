//
//  android_sound_instance.cc
//  SharkSound
//
//  Created by Jon Sharkey on 2013-06-25.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/android/android_sound_instance.h"

#include <math.h>

using namespace SharkSound;

void play_callback(SLPlayItf sl_caller_itf, void *context, SLuint32 event) {
  assert(event == SL_PLAYEVENT_HEADATEND);
  ((AndroidSoundInstance *)context)->Stop();
}

AndroidSoundInstance::AndroidSoundInstance()
    : is_busy_(false),
      sl_player_object_(NULL),
      sl_play_itf_(NULL),
      sl_volume_itf_(NULL) {
}

AndroidSoundInstance::~AndroidSoundInstance() {
  if (sl_player_object_) {
    (*sl_player_object_)->Destroy(sl_player_object_);
  }
}

bool AndroidSoundInstance::Init(SLEngineItf sl_engine_itf, SLDataSource sl_audio_source,
                                SLDataSink sl_data_sink) {
  assert(NULL == sl_player_object_);
  SLresult result;

  // create audio player
  const SLInterfaceID interface_ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
  const SLboolean interfaces_required[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
  result = (*sl_engine_itf)->CreateAudioPlayer(sl_engine_itf, &sl_player_object_, &sl_audio_source,
                                               &sl_data_sink, 3, interface_ids,
                                               interfaces_required);
  if (SL_RESULT_SUCCESS != result) return false;

  // realize the player
  result = (*sl_player_object_)->Realize(sl_player_object_, SL_BOOLEAN_FALSE);
  if (SL_RESULT_SUCCESS != result) return false;

  // get the play interface
  result = (*sl_player_object_)->GetInterface(sl_player_object_, SL_IID_PLAY, &sl_play_itf_);
  if (SL_RESULT_SUCCESS != result) return false;

  // get the volume interface
  result = (*sl_player_object_)->GetInterface(sl_player_object_, SL_IID_VOLUME, &sl_volume_itf_);
  if (SL_RESULT_SUCCESS != result) return false;

  // get the seek interface
  result = (*sl_player_object_)->GetInterface(sl_player_object_, SL_IID_SEEK, &sl_seek_itf_);
  if (SL_RESULT_SUCCESS != result) return false;

  // register a callback
  result = (*sl_play_itf_)->SetCallbackEventsMask(sl_play_itf_, SL_PLAYEVENT_HEADATEND);
  if (SL_RESULT_SUCCESS != result) return false;
  (*sl_play_itf_)->RegisterCallback(sl_play_itf_, play_callback, this);

  // enable position
  (*sl_volume_itf_)->EnableStereoPosition(sl_volume_itf_, true);

  return true;
}

void AndroidSoundInstance::Play(float volume, float position) {
  assert(NULL != sl_player_object_);
  assert(!is_busy_);

  if (volume <= 0) {
    return;
  }

  SetVolume(volume);
  SetPosition(position);
  is_busy_ = true;

  SLresult result = (*sl_play_itf_)->SetPlayState(sl_play_itf_, SL_PLAYSTATE_PLAYING);
  assert(SL_RESULT_SUCCESS == result);
}

bool AndroidSoundInstance::PlayLoop() {
  assert(NULL != sl_player_object_);
  is_busy_ = true;

  SLresult result = (*sl_seek_itf_)->SetLoop(sl_seek_itf_, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
  assert(SL_RESULT_SUCCESS == result);

  result = (*sl_play_itf_)->SetPlayState(sl_play_itf_, SL_PLAYSTATE_PLAYING);
  assert(SL_RESULT_SUCCESS == result);
}

void AndroidSoundInstance::Rewind() {
  SLresult result = (*sl_seek_itf_)->SetPosition(sl_seek_itf_, 0, SL_SEEKMODE_ACCURATE);
  assert(SL_RESULT_SUCCESS == result);
}

void AndroidSoundInstance::SetVolume(float volume) {
  if (volume <= 0) {
    (*sl_volume_itf_)->SetMute(sl_volume_itf_, SL_BOOLEAN_TRUE);
  } else {
    (*sl_volume_itf_)->SetMute(sl_volume_itf_, SL_BOOLEAN_FALSE);
    (*sl_volume_itf_)->SetVolumeLevel(sl_volume_itf_, 1500.0f * log(volume));
  }
}

void AndroidSoundInstance::SetPosition(float position) {
  (*sl_volume_itf_)->SetStereoPosition(sl_volume_itf_, 1000.0f * position);
}

void AndroidSoundInstance::Stop() {
  assert(NULL != sl_player_object_);

  SLresult result = (*sl_play_itf_)->SetPlayState(sl_play_itf_, SL_PLAYSTATE_STOPPED);
  assert(SL_RESULT_SUCCESS == result);

  is_busy_ = false;
}
