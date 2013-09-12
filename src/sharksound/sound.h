//
//  sound.h
//  SharkSound
//
//  Created by Jon Sharkey on 2013-09-08.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_SOUND_H_
#define SHARKSOUND_SOUND_H_

namespace SharkSound {

class SoundController;

class Sound {
 public:
  // Simple sound playing.
  // volume is in the range [0.0, 1.0].
  // position is the pan, and is in the range [-1.0, 1.0].
  // This allows overlapping plays of the sound.
  // Returns true on success.
  virtual bool Play(float volume = 1.f, float position = 0.f) = 0;

  // Play this sound looped. This does not support overlapping plays of this sound.
  // Returns true on success.
  virtual bool PlayLoop() = 0;
  // Manipulate the looping sound:
  virtual void StopLoop() = 0;
  virtual void RewindLoop() = 0;
  virtual void SetLoopVolume(float volume) = 0;
  virtual float LoopVolume() = 0;
  virtual void SetLoopPosition(float position) = 0;
  virtual bool IsLoopPlaying() = 0;

  // Memory management: For every call to GetSound for the same file, the retain count increases
  // by 1. This method is supplied for debugging purposes.
  int RetainCount();
  // For every call to Free, the retain count decreases by 1. When it reaches 0, the sound is freed
  // from memory.
  // Do not use this object *after* calling Free()! It means you are done with it in that context.
  void Free();

 protected:
  Sound(SoundController *sound_controller);
  virtual ~Sound();
  void IncrementRetainCount();
  void SetOn(bool on);
  void SetGlobalVolume(float volume);
  friend class SoundController;

  SoundController *sound_controller_;  // weak
  int retain_count_;
  bool on_;
  float global_volume_;
};

}

#endif
