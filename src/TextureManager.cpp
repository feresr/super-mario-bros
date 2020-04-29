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

SDL_Rect TextureManager::textureSrc(uint8_t textureId) {
    int modulo = textureId % TILESET_TILES_WIDTH;
    int division = textureId / TILESET_TILES_WIDTH;
    return SDL_Rect{
            modulo * (TILE_SIZE + TILESET_PADDING),
            division * (TILE_SIZE + TILESET_PADDING),
            TILE_SIZE,
            TILE_SIZE
    };
}

void TextureManager::renderTexture(uint8_t textureId, SDL_Rect& dstRect) {
    auto srcRect = textureSrc(textureId - 1);
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}
