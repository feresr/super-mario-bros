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
private:
    int x, y;

    ~TransformComponent() override = default;
};

struct TextureComponent : public Component {
    explicit TextureComponent(std::string path) : path{std::move(path)} {};

    const std::string path;
    SDL_Texture* texture = nullptr;

    ~TextureComponent() override {
        std::cout << "Texture component deleted" << std::endl;
        SDL_DestroyTexture(texture);
    }
};

struct CameraComponent : public Component {

    CameraComponent() = delete;

    explicit CameraComponent(int x, int y, int width, int height) : x{x}, y{y}, width{width}, height{height} {}

    int x, y, width, height;

    [[nodiscard]] int left() const {
        return x - width / 2;
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