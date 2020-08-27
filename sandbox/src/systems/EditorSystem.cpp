#include <Components.h>
#include "systems/EditorSystem.h"
#include <fstream>
#include <Log.h>

int InputWatcher(void* userData, SDL_Event* event);

void EditorSystem::onAddedToWorld(World* world) {
    SDL_AddEventWatch(InputWatcher, &inputState);

    auto* entity = world->create();
    entity->assign<TileSetComponent>(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT);
    tileSet = entity->get<TileSetComponent>();

    cursor = world->create();
    cursor->assign<Engine::TextureComponent>(TextureId::EMPTY);
    cursor->assign<TileComponent>();
    cursor->assign<Engine::TransformComponent>(0, 0, TILE_SIZE, TILE_SIZE);

    //Load current map
    std::ifstream infile("assets/world1-1", std::ios::out | std::ios::binary);
    if (infile) {
        int mw;
        int mh;
        infile.read(reinterpret_cast<char*>(&mw), sizeof(uint16_t));
        infile.read(reinterpret_cast<char*>(&mh), sizeof(uint16_t));

        world->create()->assign<TileMapComponent>(mw, mh);

        TileType texture;

        for (int x = 0; x < DEFAULT_MAP_WIDTH; x++) {
            for (int y = 0; y < DEFAULT_MAP_HEIGHT; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(TileType));
                tileSet->set(x, y, texture);
            }
        }
        infile.close();
    }
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

    auto* camera = world->findFirst<Engine::CameraComponent>()->get<Engine::CameraComponent>();
    auto selectedTileType = inputState.item % tileTypes.size();
    camera->x += inputState.pan;
    camera->x =  std::max(0, camera->x);
    if (inputState.clicked) {
        inputState.clicked = false;
        int tileX = (inputState.x + camera->left()) / TILE_SIZE;
        int tileY = (inputState.y + camera->top()) / TILE_SIZE;
        auto existing = tileSet->get(tileX, tileY).properties != NONE;
        if (existing) {
            tileSet->clear(tileX, tileY);
            ENGINE_INFO("[Editor] Tile removed");
        } else {
            tileSet->set(tileX, tileY, tileTypes[selectedTileType]);
            ENGINE_INFO("[Editor] Tile added");
        }
    }

    cursor->get<Engine::TextureComponent>()->id = tileTypes[selectedTileType].editor_texture;
    cursor->get<Engine::TransformComponent>()->x = ((inputState.x + camera->left()) / TILE_SIZE) * TILE_SIZE;
    cursor->get<Engine::TransformComponent>()->y = ((inputState.y + camera->top()) / TILE_SIZE) * TILE_SIZE;

    //Editor
//    auto tileSetEntity = world->findFirst<TileSetComponent>();
//    if (tileSetEntity) {
//        auto tileSetComponent = tileSetEntity->get<TileSetComponent>();
//        for (int x = 0; x < tileSetComponent->mapWidth; x++) {
//            for (int y = 0; y < tileSetComponent->mapHeight; y++) {
//                if (!tileSetComponent->get(x, y).texture) continue;
//                dstRect.x = x * TILE_SIZE - camera->left();
//                dstRect.y = y * TILE_SIZE - camera->top();
//                dstRect.w = TILE_SIZE;
//                dstRect.h = TILE_SIZE;
//                textureManager->renderTexture(tileSetComponent->get(x, y).editor_texture, dstRect);
//            }
//        }
//    }
}

void EditorSystem::saveToDisk() {
    std::ofstream outfile;
    outfile.open("assets/world1-1", std::ios::out | std::ios::binary);

    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapWidth)), sizeof(const uint16_t));
    outfile.write(reinterpret_cast<const char*>(&(tileSet->mapHeight)), sizeof(const uint16_t));

    for (int x = 0; x < tileSet->mapWidth; x++) {
        for (int y = 0; y < tileSet->mapHeight; y++) {
            auto tile = tileSet->get(x, y);
            outfile.write(reinterpret_cast<char*>(&tile), sizeof(TileType));
        }
    }
    ENGINE_INFO("[Texture manager] Map saved to disk: {0}");
    outfile.close();
}

EditorSystem::EditorSystem() {

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
                m->pan = -EDITOR_CAMERA_PAN_SPEED;
                break;
            case SDL_SCANCODE_D:
                m->pan = EDITOR_CAMERA_PAN_SPEED;
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
