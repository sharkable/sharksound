//
//  android_sound.cc
//  SharkSound
//
//  Created by Jon Sharkey on 2013-06-25.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/android/android_sound.h"

#include "sharksound/android/android_sound_instance.h"

using namespace SharkSound;
using std::string;

AndroidSound::AndroidSound(SoundController *sound_controller) : Sound(sound_controller) {
}

AndroidSound::~AndroidSound() {
  for (auto i = sound_instances_.begin(); i != sound_instances_.end(); i++) {
    delete *i;
  }
}

bool AndroidSound::Init(AAssetManager *asset_manager, SLEngineItf sl_engine_itf,
                        SLDataSink sl_data_sink, string filename) {
  sl_engine_itf_ = sl_engine_itf;
  sl_data_sink_ = sl_data_sink;

  assert(NULL != asset_manager);
  AAsset* asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_UNKNOWN);
  // the asset might not be found
  assert(NULL != asset);

  // open asset as file descriptor
  off_t start, length;
  int fd = AAsset_openFileDescriptor(asset, &start, &length);
  assert(0 <= fd);
  AAsset_close(asset);

  // configure audio source
  sl_data_locator_ = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
  sl_format_mime_ = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
  sl_audio_source_ = {&sl_data_locator_, &sl_format_mime_};

  AndroidSoundInstance *instance = CreateNewInstance();
  if (instance) {
    sound_instances_.push_back(instance);
  }

  return true;
}


#pragma mark - Sound

bool AndroidSound::Play(float volume, float position) {
  AndroidSoundInstance *instance = NULL;
  for (auto i = sound_instances_.begin(); i != sound_instances_.end(); i++) {
    if (!(*i)->is_busy()) {
      instance = *i;
      break;
    }
  }
  if (instance == NULL) {
    instance = CreateNewInstance();
    if (instance) {
      sound_instances_.push_back(instance);
    }
  }
  if (instance) {
    instance->Play(volume, position);
    return true;
  }
  return false;
}

bool AndroidSound::PlayLoop() {
}

bool AndroidSound::StopLoop() {
}

void AndroidSound::RewindLoop() {
}

void AndroidSound::SetLoopVolume(float volume) {
}

float AndroidSound::LoopVolume() {
}

void AndroidSound::SetLoopPosition(float position) {
}

bool AndroidSound::IsLoopPlaying() {
}


#pragma mark - private

AndroidSoundInstance * AndroidSound::CreateNewInstance() {
  AndroidSoundInstance *instance = new AndroidSoundInstance();
  if (instance->Init(sl_engine_itf_, sl_audio_source_, sl_data_sink_)) {
    return instance;
  }
  delete instance;
  return NULL;
}
