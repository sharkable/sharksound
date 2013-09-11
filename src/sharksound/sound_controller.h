//
//  sound_controller.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-01-15.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_SOUNDCONTROLLER_H_
#define SHARKSOUND_SOUNDCONTROLLER_H_

#include <map>
#include <string>

#include "sharksound/sound.h"

namespace SharkSound {

class SoundController {
 public:
  void SetSoundOn(bool on);
  void SetGlobalVolume(float volume);
  Sound * GetSound(std::string filename);

 protected:
  SoundController();
  virtual Sound * CreateSound(std::string filename) = 0;

 private:
  void FreeSound(Sound *sound);
  friend void Sound::Free();

  std::map<std::string, Sound *> sounds_;
  bool sound_on_;
  float global_volume_;
};

}

#endif
