#include "MapSystem.h"

MapSystem::MapSystem() : map{"assets/map-generated"} {
}

void MapSystem::onAddedToWorld(World* world) {
    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    if (!camera) throw std::invalid_argument("[MapSystem] Camera entity not found");
}

void MapSystem::tick(World* world) {
    // 1. Get all the entities in the world with a [TransformComponent].
    auto transformEntities = world->find<TransformComponent>();

    // 2. Iterate over them and remove the ones that are outside the camera.
    for (auto entity : transformEntities) {
        if (entity->get<TransformComponent>()->right() < camera->left()) {
            world->destroy(entity);
        }
    }

    // 3. Iterate over the `map` in the constructor and instantiate entities
    // (Remove them from the map so we don't instantiate them again)
    // TODO: (break early once we found an item that is outside the camera viewport)?
    while (!map.tiles.empty()) {
        auto tile = map.tiles.front();
        //todo: if (tile->x > camera->right()) break;
        std::cout << "[MapSystem] Adding item to world" << std::endl;
        auto entity = world->create();
        entity->assign<TransformComponent>(tile->x, tile->y, tile->w, tile->h);
        if (tile->hasProperty(VISIBLE)) entity->assign<TextureComponent>(tile->textureId);
        map.tiles.pop();
    }
}

void MapSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}

MapSystem::~MapSystem() = default;
