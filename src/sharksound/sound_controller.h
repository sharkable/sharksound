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
  // Turn on or off all sound.
  void SetSoundOn(bool on);

  // The global volume will be a multiplier on individual sound volumes.
  // Enter a value in [0.0, 1.0].
  void SetGlobalVolume(float volume);

  // Get a Sound object to play the given file. If this sound is already loaded, you'll receive
  // an existing copy. Call sound->Free() when you are finished. If GetSound("foo.wav") is called
  // twice, then Free() is called on the foo object twice, then that sound will be removed from
  // memory and the next call to GetSound will load it again.
  // On iOS, we look for the file with and without the leading path.
  // Retuns NULL if there was an error.
  Sound * GetSound(const std::string &filename);

 protected:
  SoundController();
  ~SoundController();
  virtual Sound * CreateSound(const std::string &filename) = 0;

 private:
  void FreeSound(Sound *sound);
  friend void Sound::Free();

  std::map<std::string, Sound *> sounds_;
  bool sound_on_;
  float global_volume_;
};

}

#endif
