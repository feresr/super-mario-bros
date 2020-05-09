#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer{renderer} {
    SDL_Surface* tempSurface = IMG_Load("assets/tileset.png");
    if (!tempSurface) {
        std::cout << "[Texture manager] Unable to load texture: " << IMG_GetError() << std::endl;
        throw std::invalid_argument("Unable to load texture");
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

void TextureManager::loadTextureSrc(uint8_t textureId) {
    int modulo = textureId % TILESET_TILES_WIDTH;
    int division = textureId / TILESET_TILES_WIDTH;

    srcRect.x = modulo * (TILE_SIZE + TILESET_PADDING);
    srcRect.y = division * (TILE_SIZE + TILESET_PADDING);
    srcRect.w = TILE_SIZE;
    srcRect.h = TILE_SIZE;
}

void TextureManager::renderTexture(uint8_t textureId, SDL_Rect& dstRect, bool flipH, bool flipV) {
    loadTextureSrc(textureId - 1);
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0.0, nullptr,
                     (SDL_RendererFlip) (SDL_FLIP_NONE | (SDL_FLIP_HORIZONTAL * flipH) | (SDL_FLIP_VERTICAL * flipV)));
}
