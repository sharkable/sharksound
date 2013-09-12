//
//  android_sound.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-06-25.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_ANDROID_ANDROIDSOUND_H
#define SHARKSOUND_ANDROID_ANDROIDSOUND_H

#include <android/asset_manager.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <vector>

#include "sharksound/sound.h"

namespace SharkSound {

class AndroidSoundInstance;
class SoundController;

class AndroidSound : public Sound {
 public:
  AndroidSound(SoundController *sound_controller);
  ~AndroidSound();

  bool Init(AAssetManager *asset_manager, SLEngineItf sl_engine_itf, SLDataSink sl_data_sink,
            std::string filename);

  // Sound
  bool Play(float volume = 1.f, float position = 0.f);
  bool PlayLoop();
  void StopLoop();
  void RewindLoop();
  void SetLoopVolume(float volume);
  float LoopVolume();
  void SetLoopPosition(float position);
  bool IsLoopPlaying();

 private:
  AndroidSoundInstance * CreateNewInstance();

  std::vector<AndroidSoundInstance *> sound_instances_;
  AndroidSoundInstance *loop_instance_;
  float loop_volume_;

  // OpenSL ES objects
  SLEngineItf sl_engine_itf_;
  SLDataSink sl_data_sink_;
  SLDataLocator_AndroidFD sl_data_locator_;
  SLDataFormat_MIME sl_format_mime_;
  SLDataSource sl_audio_source_;
};

}

#endif
