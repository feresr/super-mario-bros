#pragma once
#include "SDL_mixer.h"

namespace Sound {
    enum Id {
        BACKGROUND_MUSIC,
        JUMP,
        MUSHROOM
    };
}

class SoundManager {

public:
    SoundManager();

    ~SoundManager();
};


