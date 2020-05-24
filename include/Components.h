#pragma once

#include <utility>

#include "ecs/ecs.h"
#include "SDL.h"
#include "TextureManager.h"
#include "TileType.h"
#include "SoundManger.h"

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

    float w, h;
    float x, y;

    ~TransformComponent() override = default;
};

struct PlayerComponent : public Component {
};
struct SolidComponent : public Component {
};
struct CollectibleComponent : public Component {
};

struct SuperMarioComponent : public Component {
};

struct BlinkingComponent : public Component {
    explicit BlinkingComponent(int blinkSpeed) : speed{blinkSpeed} {}

    const int speed;
    int current = 0;
};

struct DestroyDelayedComponent : public Component {
    explicit DestroyDelayedComponent(int time) : timer{time} {}

    bool shouldDestroy() {
        timer--;
        return timer < 0;
    }

private:
    int timer;
};

struct KineticComponent : public Component {
    KineticComponent() {}

    KineticComponent(float accX, float accY) : speedX{accX}, speedY{accY} {}

    float speedX = 0;
    float speedY = 0;

    float accX = 0;
    float accY = 0;
};

struct GravityComponent : public Component {
};

struct CallbackComponent : public Component {
    CallbackComponent(std::function<void(void)> callback, int time) : callback{std::move(callback)}, time{time} {};

    int time;
    std::function<void(void)> callback;
};

struct FrozenComponent : public Component {
};

struct DeadComponent : public Component {
};
struct GameOverComponent : public Component {
};

struct AnimationComponent : public Component {
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

struct TextureComponent : public Component {
    explicit TextureComponent(TextureId id) : id{id} {};

    TextureId id;

    bool flipV = false;
    bool flipH = false;

    int w = 0;
    int h = 0;
    int offSetX = 0;
    int offSetY = 0;

    ~TextureComponent() override = default;
};

namespace Enemy {
    enum Type {
        GOOMBA,
        TURTLE,
        TURTLE_SHELL
    };
}

struct EnemyComponent : public Component {
    EnemyComponent(Enemy::Type type) : type{type} {}

    Enemy::Type type;
};

struct WalkComponent : public Component {
    WalkComponent() = default;

    WalkComponent(float speed) : speed{speed} {}

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

struct GrowComponent : public Component {
    [[nodiscard]] bool finished() {
        frames--;
        return frames <= 0;
    }

private:
    int frames = 64;
};

struct BreakableComponent : public Component {
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

struct MusicComponent : public Component {
    explicit MusicComponent() = default;
};

struct SoundComponent : public Component {
    explicit SoundComponent(Sound::Id sound) : sound{sound} {}

    Sound::Id sound;
};

struct TileComponent : public Component {
};

struct CrushedComponent : public Component {
};

enum Points {
    ONEHOUNDRED,
    TWOHOUNDRED,
    ONEUP,
    ONETHOUSAND
};

struct FloatingPointsComponent : public Component {
    explicit FloatingPointsComponent(Points points, int x, int y) :
            points{points}, x{x}, y{y} {}

    Points points;
    int x;
    int y;
};

struct TextComponent : public Component {
    explicit TextComponent(std::string&& text) : text{std::move(text)} {}

    std::string text;
    SDL_Texture* texture = nullptr;

    ~TextComponent() override {
        SDL_DestroyTexture(texture);
    }
};

struct AddScoreComponent : public Component {
    explicit AddScoreComponent(int score, bool addCoin = false) : score{score}, coin{addCoin} {}

    int score = 0;
    bool coin = false;
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