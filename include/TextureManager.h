#pragma once

#include <SDL_render.h>
#include <iostream>
#include <SDL_image.h>
#include <unordered_map>
#include "Constants.h"

enum TextureId {
    EMPTY,
    MARIO_STAND,
    MARIO_RUN_1,
    MARIO_RUN_2,
    MARIO_RUN_3,
    MARIO_JUMP,
    MARIO_DRIFT,
    SUPER_MARIO_STAND,
    SUPER_MARIO_RUN_1,
    SUPER_MARIO_RUN_2,
    SUPER_MARIO_RUN_3,
    SUPER_MARIO_JUMP,
    GOOMBA,
    GOOMBA_CRUSHED,
    QUESTION_BLOCK_1,
    QUESTION_BLOCK_2,
    QUESTION_BLOCK_3,
    QUESTION_BLOCK_OFF,
    MUSHROOM,
    FLOOR,
    TUBE_TOP_LEFT,
    TUBE_TOP_RIGHT,
    TUBE_RIGHT,
    TUBE_LEFT,
    CLOUD,
    BRICK,
    BLOCK,
    BUSH_RIGHT,
    BUSH_CENTER,
    BUSH_LEFT,
    BACKGROUND_CLOUD_TOP_LEFT,
    BACKGROUND_CLOUD_TOP_CENTER,
    BACKGROUND_CLOUD_TOP_RIGHT,
    BACKGROUND_CLOUD_BOTTOM_LEFT,
    BACKGROUND_CLOUD_BOTTOM_CENTER,
    BACKGROUND_CLOUD_BOTTOM_RIGHT,
};

class TextureManager {

public:
    explicit TextureManager(SDL_Renderer* renderer);

    void renderTexture(TextureId textureId, SDL_Rect& dstRect, bool flipH = false, bool flipV = false);

    SDL_Texture* texture;

    ~TextureManager();

private:

    SDL_Renderer* renderer;

    std::unordered_map<TextureId, SDL_Rect*> textures{};
};