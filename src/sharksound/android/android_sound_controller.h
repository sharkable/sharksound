//
//  android_sound_controller.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-03-21.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_ANDROID_ANDROIDSOUNDCONTROLLER_
#define SHARKSOUND_ANDROID_ANDROIDSOUNDCONTROLLER_

#include <android/asset_manager.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <map>

#include "sharksound/sound_controller.h"

namespace SharkSound {

class Sound;

class AndroidSoundController : public SoundController {
 public:
  AndroidSoundController(AAssetManager *asset_manager);
  ~AndroidSoundController();

 private:
   Sound * CreateSound(std::string filename);

   AAssetManager *asset_manager_;  // weak

   // OpenSL ES objects
   SLDataLocator_OutputMix loc_outmix_;
   SLDataSink audio_sink_;
   SLObjectItf engine_object_;
   SLEngineItf engine_engine_;
   SLObjectItf output_mix_object_;
};

}

#endif
