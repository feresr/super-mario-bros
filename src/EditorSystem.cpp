#include <fstream>
#include "EditorSystem.h"
#include "Constants.h"

int InputWatcher(void* userData, SDL_Event* event);

void EditorSystem::onAddedToWorld(World* world) {
    SDL_AddEventWatch(InputWatcher, &inputState);

    auto* entity = world->create();
    entity->assign<TileSetComponent>(MAP_WIDTH, MAP_HEIGHT);
    tileSet = entity->get<TileSetComponent>();

    cursor = world->create();
    cursor->assign<TextureComponent>(DEFAULT_TEXTURE);
    cursor->assign<TransformComponent>(0, 0, TILE_SIZE, TILE_SIZE);
}

void EditorSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<TileSetComponent>());
    world->destroy(cursor);
    SDL_DelEventWatch(InputWatcher, &inputState);
}

void EditorSystem::tick(World* world) {
    if (inputState.performSave) {
        inputState.performSave = false;
        saveToDisk();
    }

    auto* camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    camera->x += inputState.pan;
    if (inputState.clicked) {
        inputState.clicked = false;
        int tileX = (inputState.x + camera->left()) / TILE_SIZE;
        int tileY = (inputState.y + camera->top()) / TILE_SIZE;
        auto existing = tileSet->get(tileX, tileY);
        if (existing) {
            tileSet->clear(tileX, tileY);
            std::cout << "[Editor] Tile removed" << std::endl;
        } else {
            tileSet->set(tileX, tileY, cursor->get<TextureComponent>()->id);
            std::cout << "[Editor] Tile added" << std::endl;
        }
    }
    if (inputState.item != 0) {
        cursor->get<TextureComponent>()->id += inputState.item;
        inputState.item = 0;
    }

    cursor->get<TransformComponent>()->x = ((inputState.x + camera->left()) / TILE_SIZE) * TILE_SIZE;
    cursor->get<TransformComponent>()->y = ((inputState.y + camera->top()) / TILE_SIZE) * TILE_SIZE;
}

void EditorSystem::saveToDisk() {
    std::ofstream outfile;
    outfile.open("assets/map-generated", std::ios::out | std::ios::binary);

    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapWidth)), sizeof(const uint16_t));
    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapHeight)), sizeof(const uint16_t));

    for (int x = 0; x < tileSet->mapWidth; x++) {
        for (int y = 0; y < tileSet->mapHeight; y++) {
            auto tile = tileSet->get(x, y);
            outfile.write(reinterpret_cast<char*>(&tile), sizeof(uint8_t));
        }
    }
    std::cout << "[EditorSystem] Map saved to disk" << std::endl;
    outfile.close();
}


int InputWatcher(void* userData, SDL_Event* event) {
    auto* m = (InputState*) userData;

    if (event->type == SDL_MOUSEMOTION) {
        m->x = std::max(0, event->motion.x);
        m->y = std::max(0, event->motion.y);
    }
    if (event->type == SDL_MOUSEBUTTONDOWN) m->clicked = true;
    if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.scancode) {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                m->pan = 0;
                break;
            default:
                break;
        }
    }

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.scancode) {
            case SDL_SCANCODE_S:
                m->performSave = true;
                break;
            case SDL_SCANCODE_A:
                m->pan = -PAN_SPEED;
                break;
            case SDL_SCANCODE_D:
                m->pan = PAN_SPEED;
                break;
            case SDL_SCANCODE_UP:
                m->item++;
                break;
            case SDL_SCANCODE_DOWN:
                m->item--;
            default:
                break;
        }
    }
    return 0;
}
