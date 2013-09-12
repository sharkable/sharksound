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

AndroidSound::AndroidSound(SoundController *sound_controller)
    : Sound(sound_controller),
      loop_instance_(NULL),
      loop_volume_(1.f),
      sl_engine_itf_(NULL) {
}

AndroidSound::~AndroidSound() {
  for (auto i = sound_instances_.begin(); i != sound_instances_.end(); i++) {
    delete *i;
  }
}

bool AndroidSound::Init(AAssetManager *asset_manager, SLEngineItf sl_engine_itf,
                        SLDataSink sl_data_sink, std::string filename) {
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
  if (!on_) {
    return false;
  }
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
    instance->Play(volume * global_volume_, position);
    return true;
  }
  return false;
}

bool AndroidSound::PlayLoop() {
  if (!on_) {
    return false;
  }
  if (!loop_instance_) {
    loop_instance_ = CreateNewInstance();
    loop_instance_->SetVolume(loop_volume_);
  }
  if (!loop_instance_) {
    return false;
  }
  loop_instance_->PlayLoop();
}

void AndroidSound::StopLoop() {
  if (loop_instance_) {
    loop_instance_->Stop();
  }
}

void AndroidSound::RewindLoop() {
  if (loop_instance_) {
    loop_instance_->Rewind();
  }
}

void AndroidSound::SetLoopVolume(float volume) {
  loop_volume_ = volume;
  if (loop_instance_) {
    loop_instance_->SetVolume(volume * global_volume_);
  }
}

float AndroidSound::LoopVolume() {
  return loop_volume_;
}

void AndroidSound::SetLoopPosition(float position) {
  if (loop_instance_) {
    loop_instance_->SetPosition(position);
  }
}

bool AndroidSound::IsLoopPlaying() {
  return (loop_instance_ && loop_instance_->is_busy());
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
