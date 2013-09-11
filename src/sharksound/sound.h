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
  virtual bool Play(float volume = 1.f, float position = 0.f) = 0;

  // Play this sound looped.
  virtual bool PlayLoop() = 0;
  virtual bool StopLoop() = 0;
  virtual void RewindLoop() = 0;
  virtual void SetLoopVolume(float volume) = 0;
  virtual float LoopVolume() = 0;
  virtual void SetLoopPosition(float position) = 0;
  virtual bool IsLoopPlaying() = 0;

  // Memory management.
  int RetainCount();
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
