#pragma once

#include "Shader.h"
#include "Texture2D.h"

class SpriteRenderer {
public:
    explicit SpriteRenderer(Shader& shader);

    ~SpriteRenderer();

    void DrawSprite(Texture2D& texture,
                    glm::vec3 position,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f),
                    glm::vec2 texturePosition = glm::vec2(10.0f, 10.0f),
                    float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f),
                    bool flipH = false,
                    bool flipV = false
    );

private:
    Shader shader;
    unsigned int quadVAO{};

    void initRenderData();
};