#pragma once
#include "SDL_mixer.h"
#include <unordered_map>

namespace Sound {
    enum Id {
        JUMP,
        BLOCK_HIT,
        BLOCK_BREAK,
        STOMP,
        MUSHROOM_GROW,
        MUSHROOM_EAT,
        COIN,
        DEATH,
        SHRINK,
        ONE_UP,
    };
}

class SoundManager {

public:
    SoundManager();

    void playSound(Sound::Id sound);
    void playMusic();
    void stopMusic();

    ~SoundManager();

    std::unordered_map<Sound::Id, Mix_Chunk*> sounds;
};


