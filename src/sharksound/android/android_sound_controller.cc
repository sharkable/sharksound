//
//  android_sound_controller.cc
//  SharkSound
//
//  Created by Jon Sharkey on 2013-03-21.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/android/android_sound_controller.h"

#include "sharksound/android/android_sound.h"

using namespace SharkSound;
using std::string;

AndroidSoundController::AndroidSoundController(AAssetManager *asset_manager)
    : asset_manager_(asset_manager),
      engine_object_(NULL),
      engine_engine_(NULL),
      output_mix_object_(NULL) {
  SLresult result;

  const SLuint32 engine_mix_iid_count = 1;
  const SLInterfaceID engine_mix_iids[] = {SL_IID_ENGINE};
  const SLboolean engine_mix_reqs[] = {SL_BOOLEAN_TRUE};

  // Create engine.
  result = slCreateEngine(&engine_object_, 0, NULL, engine_mix_iid_count, engine_mix_iids,
                          engine_mix_reqs);
  assert(SL_RESULT_SUCCESS == result);

  // Realize the engine.
  result = (*engine_object_)->Realize(engine_object_, SL_BOOLEAN_FALSE);
  assert(SL_RESULT_SUCCESS == result);

  // Get the engine interface, which is needed in order to create other objects.
  result = (*engine_object_)->GetInterface(engine_object_, SL_IID_ENGINE, &engine_engine_);
  assert(SL_RESULT_SUCCESS == result);

  // Create output mix.
  result = (*engine_engine_)->CreateOutputMix(engine_engine_, &output_mix_object_, 0, NULL, NULL);
  assert(SL_RESULT_SUCCESS == result);

  // Realize the output mix.
  result = (*output_mix_object_)->Realize(output_mix_object_, SL_BOOLEAN_FALSE);
  assert(SL_RESULT_SUCCESS == result);

  // Configure audio sink.
  loc_outmix_ = {SL_DATALOCATOR_OUTPUTMIX, output_mix_object_};
  audio_sink_ = {&loc_outmix_, NULL};
}

AndroidSoundController::~AndroidSoundController() {
  if (output_mix_object_) {
    (*output_mix_object_)->Destroy(output_mix_object_);
  }
  if (engine_object_) {
    (*engine_object_)->Destroy(engine_object_);
  }
}


#pragma mark - private

Sound * AndroidSoundController::CreateSound(std::string filename) {
  AndroidSound *sound = new AndroidSound(this);
  if (sound->Init(asset_manager_, engine_engine_, audio_sink_, filename)) {
    return sound;
  }
  delete sound;
  return NULL;
}
