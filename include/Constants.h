#pragma once
constexpr int FPS = 60;
constexpr int ZOOM = 4;
constexpr int FRAME_DURATION = 1000 / FPS;

constexpr int SNES_RESOLUTION_WIDTH = 256;
constexpr int SNES_RESOLUTION_HEIGHT = 224;

constexpr int TILE_SIZE = 16;
constexpr int MAP_WIDTH = 100;
constexpr int MAP_HEIGHT = 14;

constexpr int TILESET_TILES_WIDTH = 13;
constexpr int TILESET_PADDING = 1;

constexpr int SKY_RED = 90;
constexpr int SKY_GREEN = 147;
constexpr int SKY_BLUE = 254;

constexpr int EDITOR_CAMERA_PAN_SPEED = 3;
constexpr int DEFAULT_TEXTURE = 1;

constexpr float GRAVITY = .05;
constexpr float FRICTION = .85;
constexpr float MAX_SPEED = 3;

constexpr float MARIO_ACCELERATION_X = .3f;
constexpr float MARIO_JUMP = 1.0f;
constexpr int TILE_ROUNDNESS = 0;