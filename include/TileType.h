#pragma once

enum Properties {
    NONE,
    VISIBLE,
    SOLID,
    MASS,
};

struct TileType {
    TileType() = default;

    TileType(uint8_t texture, uint32_t properties) : texture{texture}, properties{properties} {}

    uint8_t texture = 0;
    uint32_t properties = NONE;
};

static TileType FLOOR{1, VISIBLE | SOLID};
static TileType BUSH_LEFT{3, VISIBLE};
static TileType BUSH_CENTER{4, VISIBLE};
static TileType BUSH_RIGHT{5, VISIBLE};
static TileType BLOCK{7, VISIBLE | SOLID};
static TileType CLOUD{52, VISIBLE | SOLID | MASS};