#include "SoundManger.h"

Mix_Music* music;

SoundManager::SoundManager() {
    //Initialize SDL_mixer
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("assets/music/overworld.wav");

    Mix_PlayMusic(music, -1 );
}

SoundManager::~SoundManager() {
    Mix_PauseMusic();
    Mix_CloseAudio();
    Mix_FreeMusic(music);
}