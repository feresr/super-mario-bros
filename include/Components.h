#pragma once

#include <utility>

#include "ecs/ecs.h"
#include "SDL.h"
#include "TileType.h"

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

    int getCenterX() {
        return x + w / 2;
    }

    int getCenterY() {
        return y + h / 2;
    }

    void setTop(int value) {
        y = value;
    }

    void setBottom(int value) {
        y = value - h;
    }

    void setLeft(int value) {
        x = value;
    }

    void setRight(int value) {
        x = value - w;
    }

    const int w, h;
    int x, y;

    ~TransformComponent() override = default;
};

struct PlayerComponent : public Component {
};
struct SolidComponent : public Component {
};

struct KineticComponent : public Component {
    float speedX = 0;
    float speedY = 0;

    float accX = 0;
    float accY = 0;
};

struct GravityComponent : public Component {
};

struct TextureComponent : public Component {
    explicit TextureComponent(uint8_t id) : id{id} {};

    uint8_t id;

    ~TextureComponent() override = default;
};

struct TileComponent : public Component{};

struct TileMapComponent : public Component {
    TileMapComponent(uint16_t width, uint16_t height) : mapWidth{width},
                                                        mapHeight{height},
                                                        tiles{new Entity*[width * height]{}} {}

    Entity* get(int x, int y) {
        return tiles[x + y * mapWidth];
    }

    void set(int x, int y, Entity* value) {
        tiles[x + y * mapWidth] = value;
    }

    void clear() {
        for(int i = 0; i < mapWidth * mapHeight; i++) tiles[i] = nullptr;
    }

    const uint16_t mapWidth;
    const uint16_t mapHeight;

    ~TileMapComponent() override {
        delete[] tiles;
    }

private:
    Entity** tiles{};
};


struct TileSetComponent : public Component {
    TileSetComponent(uint16_t width, uint16_t height) : mapWidth{width},
                                                        mapHeight{height},
                                                        tiles{new TileType[width * height]{}} {}

    TileType get(int x, int y) {
        return tiles[x + y * mapWidth];
    }

    void set(int x, int y, TileType value) {
        tiles[x + y * mapWidth] = value;
    }

    void clear(int x, int y) {
        tiles[x + y * mapWidth].properties = NONE;
        tiles[x + y * mapWidth].texture = 0;
    }

    const uint16_t mapWidth;
    const uint16_t mapHeight;

    ~TileSetComponent() override {
        delete[] tiles;
    }

private:
    TileType* tiles;
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