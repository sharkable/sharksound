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

  const SLuint32 engineMixIIDCount = 1;
  const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
  const SLboolean engineMixReqs[] = {SL_BOOLEAN_TRUE};

  // create engine
  result = slCreateEngine(&engine_object_, 0, NULL, engineMixIIDCount, engineMixIIDs,
                          engineMixReqs);
  assert(SL_RESULT_SUCCESS == result);

  // realize the engine
  result = (*engine_object_)->Realize(engine_object_, SL_BOOLEAN_FALSE);
  assert(SL_RESULT_SUCCESS == result);

  // get the engine interface, which is needed in order to create other objects
  result = (*engine_object_)->GetInterface(engine_object_, SL_IID_ENGINE, &engine_engine_);
  assert(SL_RESULT_SUCCESS == result);

  // create output mix
  result = (*engine_engine_)->CreateOutputMix(engine_engine_, &output_mix_object_, 0, NULL, NULL);
  assert(SL_RESULT_SUCCESS == result);

  // realize the output mix
  result = (*output_mix_object_)->Realize(output_mix_object_, SL_BOOLEAN_FALSE);
  assert(SL_RESULT_SUCCESS == result);

  // configure audio sink
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
  return (new AndroidSound(this))->Init(asset_manager_, engine_engine_, audio_sink_, "sounds/" + filename);
}
