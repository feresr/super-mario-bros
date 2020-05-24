#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer{renderer} {
    SDL_Surface* tempSurface = IMG_Load("assets/tileset.png");
    if (!tempSurface) {
        std::cout << "[Texture manager] Unable to load texture: " << IMG_GetError() << std::endl;
        throw std::invalid_argument("Unable to load texture");
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // small mario
    textures.insert_or_assign(MARIO_STAND, new SDL_Rect{0, 254, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_RUN_1, new SDL_Rect{17, 254, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_RUN_2, new SDL_Rect{34, 254, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_RUN_3, new SDL_Rect{51, 254, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_JUMP, new SDL_Rect{85, 254, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_DRIFT, new SDL_Rect{68, 254, TILE_SIZE, TILE_SIZE});
    //big mario
    textures.insert_or_assign(MARIO_GROWING, new SDL_Rect{136, 271, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_STAND, new SDL_Rect{0, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_RUN_1, new SDL_Rect{17, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_RUN_2, new SDL_Rect{34, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_RUN_3, new SDL_Rect{51, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_JUMP, new SDL_Rect{85, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_DRIFT, new SDL_Rect{68, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(SUPER_MARIO_DUCK, new SDL_Rect{102, 221, TILE_SIZE, TILE_SIZE * 2});

    //tiles
    textures.insert_or_assign(FLOOR, new SDL_Rect{0, 0, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(QUESTION_BLOCK_1, new SDL_Rect{0, 204, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(QUESTION_BLOCK_2, new SDL_Rect{17, 204, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(QUESTION_BLOCK_3, new SDL_Rect{34, 204, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(QUESTION_BLOCK_OFF, new SDL_Rect{51, 204, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MUSHROOM, new SDL_Rect{119, 204, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(TUBE_TOP_LEFT, new SDL_Rect{0, 34, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(TUBE_TOP_RIGHT, new SDL_Rect{17, 34, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(TUBE_LEFT, new SDL_Rect{0, 51, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(TUBE_RIGHT, new SDL_Rect{17, 51, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CLOUD, new SDL_Rect{204, 51, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(GOOMBA, new SDL_Rect{119, 187, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(GOOMBA_CRUSHED, new SDL_Rect{102, 187, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(BLOCK, new SDL_Rect{34, 34, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BRICK, new SDL_Rect{85, 0, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BUSH_LEFT, new SDL_Rect{34, 0, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BUSH_CENTER, new SDL_Rect{51, 0, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BUSH_RIGHT, new SDL_Rect{68, 0, TILE_SIZE, TILE_SIZE});

    //Background clouds
    textures.insert_or_assign(BACKGROUND_CLOUD_TOP_LEFT, new SDL_Rect{0, 119, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_CLOUD_TOP_CENTER, new SDL_Rect{17, 119, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_CLOUD_TOP_RIGHT, new SDL_Rect{34, 119, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_CLOUD_BOTTOM_LEFT, new SDL_Rect{0, 136, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_CLOUD_BOTTOM_CENTER, new SDL_Rect{17, 136, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_CLOUD_BOTTOM_RIGHT, new SDL_Rect{34, 136, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(BRICK_DEBRIS_1, new SDL_Rect{136, 153, 8, 8});
    textures.insert_or_assign(BRICK_DEBRIS_2, new SDL_Rect{144, 153, 8, 8});
    textures.insert_or_assign(BRICK_DEBRIS_3, new SDL_Rect{136, 161, 8, 8});
    textures.insert_or_assign(BRICK_DEBRIS_4, new SDL_Rect{144, 161, 8, 8});

    textures.insert_or_assign(BACKGROUND_MOUNTAIN_1, new SDL_Rect{0, 17, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_MOUNTAIN_2, new SDL_Rect{17, 17, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_MOUNTAIN_3, new SDL_Rect{34, 17, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_MOUNTAIN_4, new SDL_Rect{17, 0, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(BACKGROUND_MOUNTAIN_5, new SDL_Rect{51, 17, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(COIN_1, new SDL_Rect{153, 153, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(COIN_2, new SDL_Rect{170, 153, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(COIN_3, new SDL_Rect{187, 153, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(COIN_4, new SDL_Rect{204, 153, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(TURTLE_1, new SDL_Rect{119, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(TURTLE_2, new SDL_Rect{136, 221, TILE_SIZE, TILE_SIZE * 2});
    textures.insert_or_assign(TURTLE_SHELL_1, new SDL_Rect{153, 237, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(TURTLE_SHELL_2, new SDL_Rect{170, 237, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(MARIO_DEAD, new SDL_Rect{102, 254, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(FLAG_TOP, new SDL_Rect{51, 34, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(FLAG_RIGHT, new SDL_Rect{51, 51, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(FLAG_LEFT, new SDL_Rect{34, 51, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(FLAG_POLE, new SDL_Rect{187, 119, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_1, new SDL_Rect{0, 68, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_2, new SDL_Rect{17, 68, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_3, new SDL_Rect{34, 68, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_4, new SDL_Rect{0, 85, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_5, new SDL_Rect{17, 85, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_6, new SDL_Rect{34, 85, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_DOOR, new SDL_Rect{17, 102, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(CASTLE_8, new SDL_Rect{34, 102, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(COIN_SMALL_1, new SDL_Rect{119, 254, 5, 8});
    textures.insert_or_assign(COIN_SMALL_2, new SDL_Rect{129, 254, 5, 8});
    textures.insert_or_assign(COIN_SMALL_3, new SDL_Rect{124, 262, 5, 8});

    textures.insert_or_assign(ONE_UP, new SDL_Rect{136, 204, TILE_SIZE, TILE_SIZE});

    textures.insert_or_assign(ONE_UP_LABEL, new SDL_Rect{184, 271, 16, 7});
    textures.insert_or_assign(ONEHUNDRED, new SDL_Rect{153, 287, 11, 8});
    textures.insert_or_assign(TWOHUNDRED, new SDL_Rect{153, 279, 11, 8});
    textures.insert_or_assign(THOUSAND, new SDL_Rect{153, 287, 15, 8});
    textures.insert_or_assign(MARIO_FLAG_1, new SDL_Rect{170, 170, TILE_SIZE, TILE_SIZE});
    textures.insert_or_assign(MARIO_FLAG_2, new SDL_Rect{187, 170, TILE_SIZE, TILE_SIZE});
}

void TextureManager::renderTexture(TextureId textureId, SDL_Rect& dstRect, bool flipH, bool flipV) {
    auto txt = textures.find(textureId);
    if (txt != textures.end()) {
        if (dstRect.w == 0) dstRect.w = txt->second->w;
        if (dstRect.h == 0) dstRect.h = txt->second->h;
        SDL_RenderCopyEx(renderer, texture, txt->second, &dstRect, 0.0, nullptr,
                         (SDL_RendererFlip) (SDL_FLIP_NONE | (SDL_FLIP_HORIZONTAL * flipH) |
                                             (SDL_FLIP_VERTICAL * flipV)));
    }
}

TextureManager::~TextureManager() {
    std::unordered_map<TextureId, SDL_Rect*>::iterator it;
    for (it = textures.begin(); it != textures.end(); ++it) delete it->second;
}
