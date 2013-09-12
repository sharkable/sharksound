//
//  AppleSound.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-01-15.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_APPLE_APPLESOUND_H_
#define SHARKSOUND_APPLE_APPLESOUND_H_

#include <vector>

#include "sharksound/sound.h"
#import "sharksound/apple/MyOpenALSupport.h"

namespace SharkSound {

class AppleSound : public Sound {
 public:
  AppleSound(SoundController *sound_controller, const std::string &filename);
  ~AppleSound();

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
  ALuint CreateNewSource();

  static ALCcontext *context__;
  static ALCdevice *device__;

  ALuint buffer_id_;
  std::vector<ALuint> source_ids_;
  ALuint loop_source_id_;
  float loop_volume_;
};

}

#endif
