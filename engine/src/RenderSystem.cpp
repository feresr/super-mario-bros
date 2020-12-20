#include "systems/RenderSystem.h"
#include <SDL_ttf.h>
#include "iostream"

#include "glad/glad.h"

auto dstRect = SDL_Rect();

TTF_Font* font;

namespace Engine {
    void RenderSystem::tick(World* world) {
        glClearColor(clearColor.x / 255.0f, clearColor.y / 255.0f, clearColor.z / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Todo: Render items that don't follow the camera (Te TextComponent, TransformComponent, TextureComponent
        world->find<TransformComponent, TextureComponent>([&](Entity* entity) { renderEntity(entity); });
        world->find<TextComponent, TransformComponent>([&](Entity* entity) { renderText(entity); });
    }

    void RenderSystem::onAddedToWorld(World* world) {
        auto* entity = world->create();
        entity->assign<CameraComponent>(GAME_RESOLUTION_WIDTH / 2,
                                        GAME_RESOLUTION_HEIGHT / 2,
                                        GAME_RESOLUTION_WIDTH,
                                        GAME_RESOLUTION_HEIGHT);
        camera = entity->get<CameraComponent>();

        TTF_Init();
        font = TTF_OpenFont("assets/font.ttf", 25);
        TTF_SetFontKerning(font, 1);
    }

    void RenderSystem::onRemovedFromWorld(World* world) {
        world->destroy(world->findFirst<CameraComponent>());
        TTF_CloseFont(font);
        TTF_Quit();
    }

    RenderSystem::RenderSystem(SDL_Window* window,
                               int gameResolutionWidth,
                               int gameResolutionHeight,
                               glm::vec3 clearColor)
            : GAME_RESOLUTION_WIDTH{gameResolutionWidth},
              GAME_RESOLUTION_HEIGHT{gameResolutionHeight},
              clearColor{clearColor} {


        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 3);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        glContext = SDL_GL_CreateContext(window);

        if (!glContext) {
            SDL_Log("Could not create SDL renderer: %s", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            throw std::runtime_error("Could not initialize RenderSystem");
        }
        SDL_GL_MakeCurrent(window, glContext);
        if (!gladLoadGL()) std::cout << "Failed to initialize GLAD" << std::endl;

        textureManager = new TextureManager{};
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    RenderSystem::~RenderSystem() {
        delete textureManager;
        SDL_GL_DeleteContext(glContext);
    }

    void RenderSystem::renderEntity(Entity* entity, bool followCamera) {
        auto transform = entity->get<TransformComponent>();
        auto texture = entity->get<TextureComponent>();
        if (followCamera) {
            dstRect.x = transform->left() - camera->left() + texture->offSetX;
            dstRect.y = transform->top() - camera->top() + texture->offSetY;
        } else {
            dstRect.x = transform->left() + texture->offSetX;
            dstRect.y = transform->top() + texture->offSetY;
        }

        dstRect.w = texture->w > 0 ? texture->w : transform->w;
        dstRect.h = texture->h > 0 ? texture->h : transform->h;

        textureManager->renderTexture(texture->id,
                                      dstRect,
                                      transform->z,
                                      texture->flipH,
                                      texture->flipV);
    }

    void RenderSystem::renderText(Entity* text) {
        //TODO: fix text rendering on OpenGL
        auto textComponent = text->get<TextComponent>();
        auto transformComponent = text->get<TransformComponent>();
        if (!textComponent->texture) {
            SDL_Color color = {255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, textComponent->text.c_str(), color);
            //textComponent->texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }

        dstRect.x = transformComponent->left();
        dstRect.y = transformComponent->top();

        dstRect.w = transformComponent->w;
        dstRect.h = transformComponent->h;

        //SDL_RenderCopy(renderer, textComponent->texture, nullptr, &dstRect);
    }

    void RenderSystem::setBackgroundColor(int r, int g, int b) {
        clearColor = glm::vec3{r, g, b};
    }

}