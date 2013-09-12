//
//  AppleSoundController.h
//  SharkSound
//
//  Created by Jon Sharkey on 2009-10-21.
//  Copyright 2009 Sharkable. All rights reserved.
//

#ifndef SHARKSOUND_APPLE_APPLESOUNDCONTROLLER_H_
#define SHARKSOUND_APPLE_APPLESOUNDCONTROLLER_H_

#include "sharksound/sound_controller.h"

namespace SharkSound {

class AppleSoundController : public SoundController {
 public:
  AppleSoundController();

 private:
  Sound * CreateSound(const std::string &filename);
};

}

#endif
