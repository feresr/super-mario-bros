#pragma once

#include "ecs.h"
#include "TileType.h"


struct PlayerComponent : public Engine::Component {
};
struct SolidComponent : public Engine::Component {
};
struct CollectibleComponent : public Engine::Component {
};

struct FlagPoleComponent : public Engine::Component {
};
struct FlagComponent : public Engine::Component {
};

struct SuperMarioComponent : public Engine::Component {
};

struct BlinkingComponent : public Engine::Component {
    explicit BlinkingComponent(int blinkSpeed) : speed{blinkSpeed} {}

    const int speed;
    int current = 0;
};

struct DestroyDelayedComponent : public Engine::Component {
    explicit DestroyDelayedComponent(int time) : timer{time} {}

    bool shouldDestroy() {
        timer--;
        return timer < 0;
    }

private:
    int timer;
};

struct KineticComponent : public Engine::Component {
    KineticComponent() = default;

    KineticComponent(float speedX,
                     float speedY,
                     float accX = 0.0,
                     float accY = 0.0
    ) : speedX{speedX}, speedY{speedY}, accX{accX}, accY{accY} {}
    float speedX = 0;
    float speedY = 0;

    float accX = 0;
    float accY = 0;
};

struct GravityComponent : public Engine::Component {
};

struct CallbackComponent : public Engine::Component {
    CallbackComponent(std::function<void(void)> callback, int time) : callback{std::move(callback)}, time{time} {};

    int time;
    std::function<void(void)> callback;
};

struct FrozenComponent : public Engine::Component {
};

struct DeadComponent : public Engine::Component {
};
struct GameOverComponent : public Engine::Component {
};

struct AnimationComponent : public Engine::Component {
    explicit AnimationComponent(
            std::vector<TextureId>&& textures,
            int duration,
            bool flipH = false,
            bool flipV = false,
            bool loop = true
    ) : textures{std::move(textures)},
        duration{duration},
        counter{duration},
        flipH{flipH},
        flipV{flipV},
        loop{loop} {}

    std::vector<TextureId> textures;
    int duration = 0;
    int counter = 0;
    bool flipH = false;
    bool flipV = false;
    bool loop = true;
    size_t currentTexture = 0;
};


namespace Enemy {
    enum Type {
        GOOMBA,
        TURTLE,
        TURTLE_SHELL
    };
}

struct EnemyComponent : public Engine::Component {
    explicit EnemyComponent(Enemy::Type type) : type{type} {}

    Enemy::Type type;
};

struct WalkComponent : public Engine::Component {
    WalkComponent() = default;

    explicit WalkComponent(float speed) : speed{speed} {}

    float speed = -.6;
};

enum class Direction {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

struct LeftCollisionComponent : public Engine::Component {
};
struct RightCollisionComponent : public Engine::Component {
};
struct TopCollisionComponent : public Engine::Component {
};
struct BottomCollisionComponent : public Engine::Component {
};

struct QuestionBlockComponent : public Engine::Component {
    explicit QuestionBlockComponent(
            bool spawn = false,
            bool coin = false,
            bool oneup = false
    ) : spawn{spawn}, coin{coin}, oneup{oneup} {}

    QuestionBlockComponent() = default;

    bool spawn = false;
    bool coin = false;
    bool oneup = false;
};

struct GrowComponent : public Engine::Component {
    [[nodiscard]] bool finished() {
        frames--;
        return frames <= 0;
    }

private:
    int frames = 64;
};

struct BreakableComponent : public Engine::Component {
    bool hit = false;

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

struct TileComponent : public Engine::Component {
};

struct CrushedComponent : public Engine::Component {
};

enum Points {
    ONEHOUNDRED,
    TWOHOUNDRED,
    ONEUP,
    ONETHOUSAND
};

struct FloatingPointsComponent : public Engine::Component {
    explicit FloatingPointsComponent(Points points, int x, int y) :
            points{points}, x{x}, y{y} {}

    Points points;
    int x;
    int y;
};



struct AddScoreComponent : public Engine::Component {
    explicit AddScoreComponent(int score, bool addCoin = false) : score{score}, coin{addCoin} {}

    int score = 0;
    bool coin = false;
};

struct TileMapComponent : public Engine::Component {
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


struct TileSetComponent : public Engine::Component {
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
        tiles[x + y * mapWidth].texture = TextureId::EMPTY;
    }

    const uint16_t mapWidth;
    const uint16_t mapHeight;

    ~TileSetComponent() override {
        delete[] tiles;
    }

private:
    TileType* tiles;
};

