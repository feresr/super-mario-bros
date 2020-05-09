#pragma once

#include <SDL_render.h>
#include <iostream>
#include <SDL_image.h>
#include "Constants.h"

class TextureManager {

public:
    explicit TextureManager(SDL_Renderer* renderer);

    void renderTexture(uint8_t textureId, SDL_Rect& dstRect, bool flipH = false, bool flipV = false);

    SDL_Texture* texture;
private:
    void loadTextureSrc(uint8_t textureId);

    SDL_Renderer* renderer;

    SDL_Rect srcRect{};
};