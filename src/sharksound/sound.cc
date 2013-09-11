//
//  sound.cc
//  SharkSound
//
//  Created by Jon Sharkey on 2013-09-11.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/sound.h"

#include "sharksound/sound_controller.h"

using namespace SharkSound;

int Sound::RetainCount() {
  return retain_count_;
}

void Sound::Free() {
  retain_count_--;
  if (retain_count_ <= 0) {
    sound_controller_->FreeSound(this);
  }
}


#pragma mark - protected

Sound::Sound(SoundController *sound_controller)
    : sound_controller_(sound_controller),
      retain_count_(0),
      on_(true),
      global_volume_(1.f) {
}

void Sound::IncrementRetainCount() {
  retain_count_++;
}

void Sound::SetOn(bool on) {
  on_ = on;
  if (!on_) {
    StopLoop();
  }
}

void Sound::SetGlobalVolume(float volume) {
  global_volume_ = volume;
  SetLoopVolume(LoopVolume());
}
