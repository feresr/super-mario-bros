#pragma once

#include "Constants.h"

enum Properties {
    NONE = 0,
    VISIBLE = 1,
    SOLID = 2,
    MASS = 4,
    KINETIC = 8,
    BREAKABLE = 16,
    QUESTION = 32,
};

struct TileType {
    TileType() = default;

    TileType(uint8_t texture, uint32_t properties) : texture{texture}, properties{properties} {}

    uint8_t texture = 0;
    uint32_t properties = NONE;
};

static TileType FLOOR{1, VISIBLE | SOLID};
static TileType TUBE_TOP_LEFT{2 * 13 + 1, VISIBLE | SOLID};
static TileType TUBE_TOP_RIGHT{2 * 13 + 2, VISIBLE | SOLID};
static TileType TUBE_RIGHT{3 * 13 + 1, VISIBLE | SOLID};
static TileType TUBE_LEFT{3 * 13 + 2, VISIBLE | SOLID};
static TileType QUESTION_BLOCK{QUESTION_BLOCK_TEXTURE_1, VISIBLE | SOLID | BREAKABLE | QUESTION };
static TileType BUSH_LEFT{3, VISIBLE};
static TileType BUSH_CENTER{4, VISIBLE};
static TileType BUSH_RIGHT{5, VISIBLE};
static TileType BLOCK{7, VISIBLE | SOLID};
static TileType BRICK{6, VISIBLE | SOLID | BREAKABLE};
static TileType CLOUD{52, VISIBLE | SOLID | MASS | KINETIC};
static TileType GOOMBA{11 * 13 + 8, VISIBLE | SOLID | MASS | KINETIC};