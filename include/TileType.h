#pragma once

#include "Constants.h"
#include "TextureManager.h"

enum Properties {
    NONE = 0,
    VISIBLE = 1,
    SOLID = 2,
    MASS = 4,
    KINETIC = 8,
    BREAKABLE = 16,
    QUESTION = 32,
    SPAWN = 64,
    COIN = 128,
    ENEMY = 256
};

struct TileType {
    TileType() = default;

    TileType(TextureId texture, uint32_t properties) :
            texture{texture},
            editor_texture{texture},
            properties{properties} {}

    TileType(TextureId texture, uint32_t properties, TextureId editor_texture) :
            texture{texture},
            editor_texture{editor_texture},
            properties{properties} {}

    TextureId texture = TextureId::EMPTY;
    TextureId editor_texture = texture;
    uint32_t properties = NONE;
};

namespace Tiles {
    static TileType FLOOR{TextureId::FLOOR, VISIBLE | SOLID};
    static TileType TUBE_TOP_LEFT{TextureId::TUBE_TOP_LEFT, VISIBLE | SOLID};
    static TileType TUBE_TOP_RIGHT{TextureId::TUBE_TOP_RIGHT, VISIBLE | SOLID};
    static TileType TUBE_RIGHT{TextureId::TUBE_RIGHT, VISIBLE | SOLID};
    static TileType TUBE_LEFT{TextureId::TUBE_LEFT, VISIBLE | SOLID};
    static TileType QUESTION_BLOCK{TextureId::QUESTION_BLOCK_1, VISIBLE | SOLID | BREAKABLE | QUESTION};
    static TileType QUESTION_BLOCK_MUSHROOM{TextureId::QUESTION_BLOCK_1, VISIBLE | SOLID | BREAKABLE | QUESTION | SPAWN,
                                            TextureId::MUSHROOM};
    static TileType BUSH_LEFT{TextureId::BUSH_LEFT, VISIBLE};
    static TileType BUSH_CENTER{TextureId::BUSH_CENTER, VISIBLE};
    static TileType BUSH_RIGHT{TextureId::BUSH_RIGHT, VISIBLE};
    static TileType BLOCK{TextureId::BLOCK, VISIBLE | SOLID};
    static TileType BRICK{TextureId::BRICK, VISIBLE | SOLID | BREAKABLE};
    static TileType CLOUD{TextureId::CLOUD, VISIBLE | SOLID | MASS | KINETIC};
    static TileType GOOMBA{TextureId::GOOMBA, VISIBLE | SOLID | MASS | KINETIC | ENEMY};
    static TileType TURTLE{TextureId::TURTLE_1, VISIBLE | SOLID | MASS | KINETIC | ENEMY};

    static TileType BACKGROUND_CLOUD_TOP_LEFT{TextureId::BACKGROUND_CLOUD_TOP_LEFT, VISIBLE};
    static TileType BACKGROUND_CLOUD_TOP_CENTER{TextureId::BACKGROUND_CLOUD_TOP_CENTER, VISIBLE};
    static TileType BACKGROUND_CLOUD_TOP_RIGHT{TextureId::BACKGROUND_CLOUD_TOP_RIGHT, VISIBLE};
    static TileType BACKGROUND_CLOUD_BOTTOM_LEFT{TextureId::BACKGROUND_CLOUD_BOTTOM_LEFT, VISIBLE};
    static TileType BACKGROUND_CLOUD_BOTTOM_CENTER{TextureId::BACKGROUND_CLOUD_BOTTOM_CENTER, VISIBLE};
    static TileType BACKGROUND_CLOUD_BOTTOM_RIGHT{TextureId::BACKGROUND_CLOUD_BOTTOM_RIGHT, VISIBLE};

    static TileType MOUNTAIN_1{TextureId::BACKGROUND_MOUNTAIN_1, VISIBLE};
    static TileType MOUNTAIN_2{TextureId::BACKGROUND_MOUNTAIN_2, VISIBLE};
    static TileType MOUNTAIN_3{TextureId::BACKGROUND_MOUNTAIN_3, VISIBLE};
    static TileType MOUNTAIN_4{TextureId::BACKGROUND_MOUNTAIN_4, VISIBLE};

    static TileType QUESTION_BLOCK_COIN{TextureId::QUESTION_BLOCK_1,
                                        VISIBLE | SOLID | BREAKABLE | QUESTION | COIN,
                                            TextureId::COIN_1};
}
