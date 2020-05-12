#pragma once

#include <utility>

#include "ecs/ecs.h"
#include "SDL.h"
#include "TileType.h"

struct TransformComponent : public Component {

    TransformComponent(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

    [[nodiscard]] float right() const {
        return x + w;
    }

    [[nodiscard]] float left() const {
        return x;
    }

    [[nodiscard]] float top() const {
        return y;
    }

    [[nodiscard]] float bottom() const {
        return y + h;
    }

    [[nodiscard]] float getCenterX() const {
        return x + w / 2.0f;
    }

    [[nodiscard]] float getCenterY() const {
        return y + h / 2.0f;
    }

    void setTop(float value) {
        y = value;
    }

    void setBottom(float value) {
        y = value - h;
    }

    void setLeft(float value) {
        x = value;
    }

    void setRight(float value) {
        x = value - w;
    }

    const float w, h;
    float x, y;

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

struct AnimationComponent : public Component {
    explicit AnimationComponent(
            std::vector<int>&& textures,
            int duration,
            bool flipH = false,
            bool flipV = false
    ) : textures{std::move(textures)}, duration{duration}, counter{duration}, flipH{flipH}, flipV{flipV} {}

    std::vector<int> textures;
    int duration = 0;
    int counter = 0;
    bool flipH = false;
    bool flipV = false;
    size_t currentTexture = 0;
};

struct TextureComponent : public Component {
    explicit TextureComponent(uint8_t id) : id{id} {};

    uint8_t id;

    bool flipV = false;
    bool flipH = false;
    ~TextureComponent() override = default;
};

struct EnemyComponent : public Component {
};
struct WalkComponent : public Component {
    float speed = -.6;
};

enum class Direction {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

struct LeftCollisionComponent : public Component {
};
struct RightCollisionComponent : public Component {
};
struct TopCollisionComponent : public Component {
};
struct BottomCollisionComponent : public Component {
};

struct QuestionBlockComponent : public Component {
};

struct BreakableComponent : public Component {
    int getHeight() {
        frames--;
        return height[frames];
    }

    [[nodiscard]] bool finished() const {
        return frames == 0;
    }

    void reset() {
        frames = 13;
    }

private:
    int frames = 13;
    int height[13] = {-2, 1, 3, 2, 1, 1, 1, 0, 0, -1, -1, -4, -1};
};

struct TileComponent : public Component {
};

struct TileMapComponent : public Component {
    TileMapComponent(uint16_t width, uint16_t height) : mapWidth{width},
                                                        mapHeight{height},
                                                        tiles{new Entity* [width * height]{}} {}

    Entity* get(int x, int y) {
        if (x < 0 || y < 0) return nullptr;
        if (x >= mapWidth || y >= mapHeight) return nullptr;
        return tiles[x + y * mapWidth];
    }

    void set(int x, int y, Entity* value) {
        tiles[x + y * mapWidth] = value;
    }

    void clear() {
        for (int i = 0; i < mapWidth * mapHeight; i++) tiles[i] = nullptr;
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