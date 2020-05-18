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
    if (player)camera->x = std::max(camera->x, (int) player->get<TransformComponent>()->getCenterX());
    auto tileMap = world->findFirst<TileMapComponent>()->get<TileMapComponent>();


    // 2. Iterate over them and remove the ones that are outside the camera.
    for (auto entity : world->find<TransformComponent>()) {
        auto transform = entity->get<TransformComponent>();
        if (transform->right() < camera->left() - CAMERA_WORLD_OFFSET
            || transform->left() > camera->right() + CAMERA_WORLD_OFFSET
            || transform->top() > camera->bottom()
                ) {
            tileMap->set(
                    (int) (transform->getCenterX() / TILE_SIZE),
                    (int) (transform->getCenterY() / TILE_SIZE),
                    nullptr
            );
            world->destroy(entity);
        }
    }

    // 3. Iterate over the `map` in the constructor and instantiate entities
    // Remove them from the map so they don't get instantiated again.
    while (!map.tiles.empty()) {
        auto tile = map.tiles.front();
        if (tile->x > camera->right() + CAMERA_WORLD_OFFSET) break;
        auto entity = world->create();
        entity->assign<TransformComponent>(tile->x, tile->y, tile->w, tile->h);
        if (tile->hasProperty(VISIBLE))  {
            entity->assign<TextureComponent>(tile->textureId);
        }
        if (tile->hasProperty(MASS)) entity->assign<GravityComponent>();
        if (tile->hasProperty(BREAKABLE)) entity->assign<BreakableComponent>();
        if (tile->hasProperty(QUESTION)) {
            entity->assign<QuestionBlockComponent>(
                    tile->hasProperty(SPAWN),
                    tile->hasProperty(COIN)
            );
            entity->assign<AnimationComponent>(
                    std::vector<TextureId>{
                            TextureId::QUESTION_BLOCK_1,
                            TextureId::QUESTION_BLOCK_1,
                            TextureId::QUESTION_BLOCK_1,
                            TextureId::QUESTION_BLOCK_2,
                            TextureId::QUESTION_BLOCK_3,
                            TextureId::QUESTION_BLOCK_2
                    },
                    10);
        }

        if (tile->hasProperty(ENEMY)) {

            if (tile->textureId == TextureId::GOOMBA) {
                entity->assign<WalkComponent>();
                entity->assign<AnimationComponent>(std::vector<TextureId>{tile->textureId},
                                                   15,
                                                   true);
                entity->assign<EnemyComponent>(Enemy::Type::GOOMBA);
            }
            if (tile->textureId == TextureId::TURTLE_1) {
                entity->assign<WalkComponent>(-.4f);
                entity->assign<AnimationComponent>(std::vector<TextureId>{
                                                           TextureId::TURTLE_1,
                                                           TextureId::TURTLE_2,
                                                   },
                                                   10,
                                                   false);
                entity->get<TextureComponent>()->h = TILE_SIZE * 2;
                entity->get<TextureComponent>()->offSetY = -TILE_SIZE;
                entity->assign<EnemyComponent>(Enemy::Type::TURTLE);
            }
        }
        if (tile->hasProperty(KINETIC)) {
            entity->assign<KineticComponent>();
        } else {
            entity->assign<TileComponent>();
            auto transform = entity->get<TransformComponent>();
            tileMap->set(
                    (int) (transform->getCenterX() / TILE_SIZE),
                    (int) (transform->getCenterY() / TILE_SIZE),
                    entity
            );
        }
        if (tile->hasProperty(SOLID)) entity->assign<SolidComponent>();
        map.tiles.pop();
        delete tile;
    }
}

MapSystem::~MapSystem() = default;
