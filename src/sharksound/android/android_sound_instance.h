//
//  android_sound_instance.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-06-25.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_ANDROID_ANDROIDSOUNDINSTANCE_H
#define SHARKSOUND_ANDROID_ANDROIDSOUNDINSTANCE_H

#include <SLES/OpenSLES.h>

namespace SharkSound {

class AndroidSoundInstance {
 public:
  AndroidSoundInstance();
  ~AndroidSoundInstance();

  AndroidSoundInstance *Init(SLEngineItf sl_engine_itf, SLDataSource sl_audio_source,
                             SLDataSink sl_data_sink);
  void Play(float volume, float position);
  void Stop();

  bool is_busy() { return is_busy_; }

 private:
  bool is_busy_;

  // OpenSL ES objects
  SLObjectItf sl_player_object_;
  SLPlayItf sl_play_itf_;
  SLVolumeItf sl_volume_itf_;
};

}

#endif
