#pragma once

#include "TextureManager.h"
#include "Component.h"
#include "SoundManger.h"
#include "string"

namespace Engine {
    struct TransformComponent : public Component {

        //TODO: redundant int to float conversion?
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

    struct MusicComponent : public Component {
        explicit MusicComponent(Music::Id music) : music{music} {};
        Music::Id music{};
    };

    struct SoundComponent : public Component {
        explicit SoundComponent(Sound::Id sound) : sound{sound} {}
        Engine::Sound::Id sound{};
    };

    struct TextComponent : public Component {
        explicit TextComponent(std::string&& text) : text{std::move(text)} {}

        std::string text;
        SDL_Texture* texture = nullptr;

        ~TextComponent() override {
            SDL_DestroyTexture(texture);
        }
    };
}

