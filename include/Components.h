#pragma once

#include <utility>

#include "ecs/ecs.h"
#include "SDL.h"

struct TransformComponent : public Component {

    TransformComponent(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

    [[nodiscard]] int right() const {
        return x + w;
    }

    [[nodiscard]] int left() const {
        return x;
    }

    [[nodiscard]] int top() const {
        return y;
    }

    [[nodiscard]] int bottom() const {
        return y + h;
    }

    const int w, h;
    int x, y;

    ~TransformComponent() override = default;
};

struct TextureComponent : public Component {
    explicit TextureComponent(uint8_t id) : id{id} {};

    uint8_t id;

    ~TextureComponent() override = default;
};

struct TileSetComponent : public Component {
    TileSetComponent(uint16_t width, uint16_t height) : mapWidth{width},
                                                        mapHeight{height},
                                                        tiles{new uint8_t[width * height]{}} {}

    const uint16_t mapWidth;
    const uint16_t mapHeight;

    uint8_t get(int x, int y) {
        return tiles[x + y * mapWidth];
    }

    void set(int x, int y, uint8_t value) {
        tiles[x + y * mapWidth] = value;
    }

    void clear(int x, int y) {
        tiles[x + y * mapWidth] = 0;
    }

    ~TileSetComponent() override {
        delete[] tiles;
    }

private:
    uint8_t* tiles;
};

struct CameraComponent : public Component {

    CameraComponent() = delete;

    explicit CameraComponent(int x, int y, int width, int height) : x{x}, y{y}, width{width}, height{height} {}

    int x, y, width, height;

    [[nodiscard]] int left() const {
        return std::max(0, x - width / 2);
    }

    [[nodiscard]] int right() const {
        return x + width / 2;
    }

    [[nodiscard]] int top() const {
        return y - height / 2;
    }

    [[nodiscard]] int bottom() const {
        return y + height / 2;
    }

};