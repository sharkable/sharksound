//
//  sound_controller.cc
//  SharkSound
//
//  Created by Jon Sharkey on 2013-09-11.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharksound/sound_controller.h"

using namespace SharkSound;

void SoundController::SetSoundOn(bool on) {
  sound_on_ = on;
  for (auto i = sounds_.begin(); i != sounds_.end(); i++) {
    i->second->SetOn(sound_on_);
  }
}

void SoundController::SetGlobalVolume(float volume) {
  global_volume_ = volume;
  for (auto i = sounds_.begin(); i != sounds_.end(); i++) {
    i->second->SetGlobalVolume(global_volume_);
  }
}

Sound * SoundController::GetSound(const std::string &filename) {
  auto i = sounds_.find(filename);
  if (i != sounds_.end()) {
    i->second->IncrementRetainCount();
    return i->second;
  }
  Sound *sound = CreateSound(filename);
  if (sound) {
    sound->IncrementRetainCount();
    sound->SetGlobalVolume(global_volume_);
    sound->SetOn(sound_on_);
    sounds_[filename] = sound;
  }
  return sound;
}


#pragma mark - Protected

SoundController::SoundController()
    : sound_on_(true),
      global_volume_(1.f) {
}

SoundController::~SoundController() {
  for (auto i = sounds_.begin(); i != sounds_.end(); i++) {
    delete i->second;
  }
}


#pragma mark - Private

void SoundController::FreeSound(Sound *sound) {
  for (auto i = sounds_.begin(); i != sounds_.end(); i++) {
    if (i->second == sound) {
      sounds_.erase(i);
      break;
    }
  }
}
