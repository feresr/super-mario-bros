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
        ONE_UP
    };
}

namespace Music {
    enum Id {
        NONE,
        BACKGROUND,
        LEVEL_END
    };
}

class SoundManager {

public:
    SoundManager();

    void playSound(Sound::Id sound);
    void playMusic(Music::Id sound);
    void stopMusic();

    ~SoundManager();

    std::unordered_map<Sound::Id, Mix_Chunk*> sounds;
    std::unordered_map<Music::Id, Mix_Music*> musics;
};


