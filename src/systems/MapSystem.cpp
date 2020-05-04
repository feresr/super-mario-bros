#include "systems/MapSystem.h"

MapSystem::MapSystem() : map{"assets/map-generated"} {
}

void MapSystem::onAddedToWorld(World* world) {
    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    if (!camera) throw std::invalid_argument("[MapSystem] Camera entity not found");
    world->create()->assign<TileMapComponent>(map.mapWidth, map.mapHeight);
}

void MapSystem::onRemovedFromWorld(World* world) {
    world->destroy(world->findFirst<TileMapComponent>());
    System::onRemovedFromWorld(world);
}

void MapSystem::tick(World* world) {
    auto player = world->findFirst<PlayerComponent, TransformComponent>();
    camera->x = std::max(camera->x, player->get<TransformComponent>()->getCenterX());
    auto tileMap = world->findFirst<TileMapComponent>()->get<TileMapComponent>();

    // 1. Get all the entities in the world with a [TileComponent, TransformComponent].
    auto transformEntities = world->find<TileComponent, TransformComponent>();

    // 2. Iterate over them and remove the ones that are outside the camera.
    for (auto entity : transformEntities) {
        auto transform = entity->get<TransformComponent>();
        if (transform->right() < camera->left() || transform->top() > camera->bottom()) {
            tileMap->set(transform->x / TILE_SIZE, transform->y / TILE_SIZE, nullptr);
            world->destroy(entity);
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
        if (tile->hasProperty(BREAKABLE)) entity->assign<BreakableComponent>();
        if (tile->hasProperty(KINETIC)) {
            entity->assign<KineticComponent>();
        } else {
            // 4. Each static `TileComponent` reports its location in the world to be accessed by X and Y coordinates.
            entity->assign<TileComponent>();
            auto transform = entity->get<TransformComponent>();
            tileMap->set(transform->x / TILE_SIZE, transform->y / TILE_SIZE, entity);
        }
        if (tile->hasProperty(SOLID)) entity->assign<SolidComponent>();
        map.tiles.pop();
    }
}

MapSystem::~MapSystem() = default;
