#pragma once

#include <SDL_render.h>
#include <iostream>
#include <SDL_image.h>
#include "Constants.h"

class TextureManager {

public:
    explicit TextureManager(SDL_Renderer* renderer);

    void renderTexture(uint8_t textureId, SDL_Rect& dstRect);

    SDL_Texture* texture;
private:
    SDL_Rect textureSrc(uint8_t textureId);

    SDL_Renderer* renderer;
};