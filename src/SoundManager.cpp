#include "SoundManger.h"

Mix_Music* music;

SoundManager::SoundManager() {
    //Initialize SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    music = Mix_LoadMUS("assets/music/overworld.wav");

    sounds.insert_or_assign(Sound::Id::JUMP, Mix_LoadWAV("assets/music/jump.wav"));
    sounds.insert_or_assign(Sound::Id::BLOCK_HIT, Mix_LoadWAV("assets/music/blockhit.wav"));
    sounds.insert_or_assign(Sound::Id::BLOCK_BREAK, Mix_LoadWAV("assets/music/blockbreak.wav"));
    sounds.insert_or_assign(Sound::Id::STOMP, Mix_LoadWAV("assets/music/stomp.wav"));
    sounds.insert_or_assign(Sound::Id::MUSHROOM_EAT, Mix_LoadWAV("assets/music/mushroomeat.wav"));
    sounds.insert_or_assign(Sound::Id::MUSHROOM_GROW, Mix_LoadWAV("assets/music/mushroomappear.wav"));
    sounds.insert_or_assign(Sound::Id::COIN, Mix_LoadWAV("assets/music/coin.wav"));
    sounds.insert_or_assign(Sound::Id::DEATH, Mix_LoadWAV("assets/music/death.wav"));
    sounds.insert_or_assign(Sound::Id::SHRINK, Mix_LoadWAV("assets/music/shrink.wav"));


}

void SoundManager::playSound(Sound::Id sound) {
    auto chunk = sounds.at(sound);
    Mix_VolumeChunk(chunk, 100);
    Mix_PlayChannel(-1, chunk, 0);
}

void SoundManager::playMusic() {
    if (!Mix_PlayingMusic()) Mix_PlayMusic(music, -1);
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
}

SoundManager::~SoundManager() {
    Mix_PauseMusic();
    Mix_CloseAudio();
    Mix_FreeMusic(music);
}