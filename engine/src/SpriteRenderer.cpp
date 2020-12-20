#include <SpriteRenderer.h>
#include <Log.h>
#include "glad/glad.h"

void SpriteRenderer::initRenderData() {
    unsigned int VBO;
    float vertices[] = {
            // pos      // tex
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };
    unsigned int TextVBO;
    float indices[] = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &TextVBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::SpriteRenderer(Shader& shader) : shader{shader} {
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() = default;

void SpriteRenderer::DrawSprite(Texture2D& texture,
                                glm::vec3 position,
                                glm::vec2 size,
                                glm::vec2 texturePosition,
                                float rotate,
                                glm::vec3 color,
                                bool flipH,
                                bool flipV
) {
    shader.Use();
    glm::mat4 texTransform = glm::mat4(1.0f);

    texTransform = glm::translate(texTransform,
                                  glm::vec3(glm::vec2(texturePosition / glm::vec2(texture.Width, texture.Height)),
                                            1.0f));
    texTransform = glm::scale(texTransform,
                              glm::vec3(glm::vec2(size) / glm::vec2(texture.Width, texture.Height), 1.0f));


    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(glm::vec2(size.x, size.y), 1.0f));
    if (flipH) {
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
    }
    if (flipV) {
        model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    }

    shader.SetMatrix4("model", model);
    shader.SetMatrix4("texture", texTransform);
    shader.SetVector3f("spriteColor", color);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
