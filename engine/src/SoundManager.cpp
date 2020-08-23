#include "SoundManger.h"
#include <SDL_mixer.h>
#include <unordered_map>

std::unordered_map<Engine::Sound::Id, Mix_Chunk*> sounds;
std::unordered_map<Engine::Music::Id, Mix_Music*> musics;

Engine::SoundManager::SoundManager() {
    //Initialize SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    sounds.insert_or_assign(Sound::Id::JUMP, Mix_LoadWAV("assets/music/jump.wav"));
    sounds.insert_or_assign(Sound::Id::BLOCK_HIT, Mix_LoadWAV("assets/music/blockhit.wav"));
    sounds.insert_or_assign(Sound::Id::BLOCK_BREAK, Mix_LoadWAV("assets/music/blockbreak.wav"));
    sounds.insert_or_assign(Sound::Id::STOMP, Mix_LoadWAV("assets/music/stomp.wav"));
    sounds.insert_or_assign(Sound::Id::MUSHROOM_EAT, Mix_LoadWAV("assets/music/mushroomeat.wav"));
    sounds.insert_or_assign(Sound::Id::MUSHROOM_GROW, Mix_LoadWAV("assets/music/mushroomappear.wav"));
    sounds.insert_or_assign(Sound::Id::COIN, Mix_LoadWAV("assets/music/coin.wav"));
    sounds.insert_or_assign(Sound::Id::DEATH, Mix_LoadWAV("assets/music/death.wav"));
    sounds.insert_or_assign(Sound::Id::SHRINK, Mix_LoadWAV("assets/music/shrink.wav"));
    sounds.insert_or_assign(Sound::Id::ONE_UP, Mix_LoadWAV("assets/music/oneup.wav"));

    musics.insert_or_assign(Music::Id::BACKGROUND, Mix_LoadMUS("assets/music/overworld.wav"));
    musics.insert_or_assign(Music::Id::LEVEL_END, Mix_LoadMUS("assets/music/levelend.wav"));
}

void Engine::SoundManager::playSound(Sound::Id sound) {
    auto chunk = sounds.at(sound);
    Mix_VolumeChunk(chunk, 100);
    Mix_PlayChannel(-1, chunk, 0);
}

void Engine::SoundManager::playMusic(Music::Id sound) {
    stopMusic();
    if (sound == Music::Id::NONE) return;
    auto loops = -1;
    if (sound == Music::Id::LEVEL_END) loops = 1;
    Mix_PlayMusic(musics.at(sound), loops);
}

void Engine::SoundManager::stopMusic() {
    Mix_HaltMusic();
}

Engine::SoundManager::~SoundManager() {
    Mix_PauseMusic();
    Mix_CloseAudio();
    for (auto s : sounds) Mix_FreeChunk(s.second);
    for (auto m : musics) Mix_FreeMusic(m.second);
}