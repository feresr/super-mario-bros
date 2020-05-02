#include "systems/MapSystem.h"

MapSystem::MapSystem() : map{"assets/map-generated"} {
}

void MapSystem::onAddedToWorld(World* world) {
    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    if (!camera) throw std::invalid_argument("[MapSystem] Camera entity not found");
}

void MapSystem::tick(World* world) {
    auto player = world->findFirst<PlayerComponent, TransformComponent>();
    camera->x = std::max(camera->x, player->get<TransformComponent>()->getCenterX());

    // 1. Get all the entities in the world with a [TransformComponent].
    auto transformEntities = world->find<TransformComponent>();
    // 2. Iterate over them and remove the ones that are outside the camera.
    for (auto entity : transformEntities) {
        if (entity->get<TransformComponent>()->right() < camera->left()
            || entity->get<TransformComponent>()->top() > camera->bottom()) {
            if (!entity->get<PlayerComponent>()) world->destroy(entity);
        }
    }

    // 3. Iterate over the `map` in the constructor and instantiate entities
    // Remove them from the map so they don't get instantiated again.
    while (!map.tiles.empty()) {
        auto tile = map.tiles.front();
        if (tile->x > camera->right()) break;
        std::cout << "[MapSystem] Adding item to world" << std::endl;
        auto entity = world->create();
        entity->assign<TransformComponent>(tile->x, tile->y, tile->w, tile->h);
        if (tile->hasProperty(VISIBLE)) entity->assign<TextureComponent>(tile->textureId);
        if (tile->hasProperty(MASS)) entity->assign<GravityComponent>();
        if (tile->hasProperty(KINETIC)) entity->assign<KineticComponent>();
        if (tile->hasProperty(SOLID)) entity->assign<SolidComponent>();
        map.tiles.pop();
    }
}

void MapSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}

MapSystem::~MapSystem() = default;
