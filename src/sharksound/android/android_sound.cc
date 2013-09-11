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

namespace SharkSound {

struct SoundInstanceNode {
  AndroidSoundInstance *value;
  SoundInstanceNode *next;

  SoundInstanceNode()
    : value(NULL),
      next(NULL) {
  }

  ~SoundInstanceNode() {
    if (value) {
      delete value;
    }
    if (next) {
      delete next;
    }
  }
};

}

AndroidSound::AndroidSound(SoundController *sound_controller) : Sound(sound_controller) {
}

AndroidSound::~AndroidSound() {
  if (sound_instance_list_head_) {
    delete sound_instance_list_head_;
  }
}

AndroidSound * AndroidSound::Init(AAssetManager *asset_manager, SLEngineItf sl_engine_itf,
                                  SLDataSink sl_data_sink, string filename) {
                                    s_log("loading: %s", filename.c_str());
  sl_engine_itf_ = sl_engine_itf;
  sl_data_sink_ = sl_data_sink;

  s_log("here A");
  assert(NULL != asset_manager);
  AAsset* asset = AAssetManager_open(asset_manager, filename.c_str(), AASSET_MODE_UNKNOWN);
  // the asset might not be found
  assert(NULL != asset);

  s_log("here B");
  // open asset as file descriptor
  off_t start, length;
  int fd = AAsset_openFileDescriptor(asset, &start, &length);
  assert(0 <= fd);
  AAsset_close(asset);

  s_log("here C");

  // configure audio source
  sl_data_locator_ = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
  sl_format_mime_ = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
  sl_audio_source_ = {&sl_data_locator_, &sl_format_mime_};

  s_log("here D");

  AddSoundInstance(&sound_instance_list_head_);

  s_log("here E");

  return this;
}


#pragma mark - Sound

bool AndroidSound::Play(float volume, float position) {
  SoundInstanceNode *non_busy_node = sound_instance_list_head_;
  while (non_busy_node->next && non_busy_node->value->is_busy()) {
    non_busy_node = non_busy_node->next;
  }
  if (non_busy_node->value->is_busy()) {
    AddSoundInstance(&non_busy_node->next);
    non_busy_node = non_busy_node->next;
  }
  if (non_busy_node) {
    non_busy_node->value->Play(volume, position);
  }
  return true;
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

void AndroidSound::AddSoundInstance(SoundInstanceNode **node) {
  *node = new SoundInstanceNode();
  (*node)->value =
      (new AndroidSoundInstance())->Init(sl_engine_itf_, sl_audio_source_, sl_data_sink_);
  if (!(*node)->value) {
    delete *node;
    *node = NULL;
  }
}
