#include "systems/PhysicsSystem.h"
#include "Components.h"
#include "AABB.h"
#include <unordered_set>
#include "Constants.h"

Direction checkCollisionY(Entity* solid, Engine::TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<Engine::TransformComponent>();
    auto direction = Direction::NONE;

    if (kinetic->speedY >= 0.0f) {
        // Falling
        if (AABBCollision(
                transform->x + TILE_ROUNDNESS / 2,    // Check previous x position
                transform->y + kinetic->speedY,
                transform->w - TILE_ROUNDNESS,
                transform->h,
                solidTransform)) {
            float distanceTop = abs(solidTransform->top() - (transform->bottom() + kinetic->speedY));
            float distanceBottom = abs((transform->top() + kinetic->speedY) - solidTransform->bottom());
            if (distanceTop < distanceBottom) {
                transform->setBottom(solidTransform->top());
                solid->assign<TopCollisionComponent>();
                direction = Direction::BOTTOM;
            }
        }
    } else {
        // Jumping
        if (AABBCollision(
                transform->x + TILE_ROUNDNESS,    // Check previous x position
                transform->y + kinetic->speedY,
                transform->w - (TILE_ROUNDNESS * 2),
                transform->h,
                solidTransform)) {
            float distanceTop = abs(solidTransform->top() - (transform->bottom() + kinetic->speedY));
            float distanceBottom = abs((transform->top() + kinetic->speedY) - solidTransform->bottom());
            if (distanceTop > distanceBottom) {
                transform->setTop(solidTransform->bottom());
                solid->assign<BottomCollisionComponent>();
                direction = Direction::TOP;
            }
        }
    }
    return direction;
}

Direction checkCollisionX(Entity* solid, Engine::TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<Engine::TransformComponent>();
    auto direction = Direction::NONE;

    // X-AXIS CHECK
    if (AABBCollision(
            transform->x + kinetic->speedX,
            transform->y + kinetic->speedY + 2,   // Check with updated y position
            transform->w,
            transform->h - 8, //Ideally should be -4, but this favours Mario ending up on TOP of a tile.
            solidTransform)) {

        float distanceLeft = abs((transform->left() + kinetic->speedX) - solidTransform->right());
        float distanceRight = abs((transform->right() + kinetic->speedX) - solidTransform->left());
        if (distanceLeft < distanceRight) {
            if (transform->left() < solidTransform->right()) {
                //item is inside block, push out
                transform->x += std::min(.5f, solidTransform->right() - transform->left());
            } else {
                //item about to get inside the block
                transform->setLeft(solidTransform->right());
            }
            solid->assign<RightCollisionComponent>();
            direction = Direction::LEFT;
        } else {
            if (transform->right() > solidTransform->left()) {
                //item is inside block, push out
                transform->x -= std::min(.5f, transform->right() - solidTransform->left());
            } else {
                //item about to get inside the block
                transform->setRight(solidTransform->left());
            }

            solid->assign<LeftCollisionComponent>();
            direction = Direction::RIGHT;
        }
    }
    return direction;
}

void PhysicsSystem::tick(World* world) {
    world->find<GravityComponent, KineticComponent>([&](Entity* entity) {
        entity->get<KineticComponent>()->accY += GRAVITY;
    });

    // Kinetic-Kinetic collisions
    world->find<Engine::TransformComponent, KineticComponent, SolidComponent>([&](Entity* entity) {
        if (entity->has<BlinkingComponent>()) return;
        auto transform = entity->get<Engine::TransformComponent>();
        auto kinetic = entity->get<KineticComponent>();
        world->find<Engine::TransformComponent, KineticComponent, SolidComponent>([&](Entity* other) {
            if (entity == other) return;
            if (!other->has<SolidComponent>()) return;
            if (other->has<CollectibleComponent>()) return;
            if (other->has<BlinkingComponent>()) return;
            switch (checkCollisionY(other, transform, kinetic)) {
                case Direction::TOP:
                    entity->assign<TopCollisionComponent>();
                    break;
                case Direction::BOTTOM:
                    entity->assign<BottomCollisionComponent>();
                    break;
                default:
                    break;
            }
            switch (checkCollisionX(other, transform, kinetic)) {
                case Direction::LEFT:
                    entity->assign<LeftCollisionComponent>();
                    break;
                case Direction::RIGHT:
                    entity->assign<RightCollisionComponent>();
                    break;
                default:
                    break;
            }
        });
    });



    // Check Kinetic-Tiles Collisions
    auto tileMapEntity = world->findFirst<TileMapComponent>();
    if (tileMapEntity) {
        auto tileMapComponent = tileMapEntity->get<TileMapComponent>();
        world->find<KineticComponent, Engine::TransformComponent, SolidComponent>([&](Entity* entity) {
            auto transform = entity->get<Engine::TransformComponent>();
            auto kinetic = entity->get<KineticComponent>();

            int futureLeft = (int) (transform->left() + kinetic->speedX) / TILE_SIZE;
            int futureRight = (int) (transform->right() + kinetic->speedX) / TILE_SIZE;
            int futureTop = (int) (transform->top() + kinetic->speedY) / TILE_SIZE;
            int futureBottom = (int) (transform->bottom() + kinetic->speedY) / TILE_SIZE;
            std::unordered_set<Entity*> coordinates{
                    tileMapComponent->get(futureRight, futureBottom),
                    tileMapComponent->get(futureLeft, futureBottom),
                    tileMapComponent->get(futureLeft, futureTop),
                    tileMapComponent->get(futureRight, futureTop),
            };
            for (auto tile : coordinates) {
                if (tile == entity) continue;
                if (!tile) continue;
                if (!(tile->get<SolidComponent>())) continue;
                switch (checkCollisionY(tile, transform, kinetic)) {
                    case Direction::TOP:
                        kinetic->accY = std::max(0.0f, kinetic->accY);
                        kinetic->speedY = std::max(0.0f, kinetic->speedY);
                        entity->assign<TopCollisionComponent>();
                        break;
                    case Direction::BOTTOM:
                        kinetic->accY = std::min(0.0f, kinetic->accY);
                        kinetic->speedY = std::min(0.0f, kinetic->speedY);
                        entity->assign<BottomCollisionComponent>();
                        break;
                    default:
                        break;
                }
            }

            futureLeft = (int) (transform->left() + kinetic->speedX) / TILE_SIZE;
            futureRight = (int) (transform->right() + kinetic->speedX) / TILE_SIZE;
            futureTop = (int) (transform->top() + 1) / TILE_SIZE;
            futureBottom = (int) (transform->bottom() - 1) / TILE_SIZE;
            coordinates = {
                    tileMapComponent->get(futureRight, futureBottom),
                    tileMapComponent->get(futureLeft, futureBottom),
                    tileMapComponent->get(futureLeft, futureTop),
                    tileMapComponent->get(futureRight, futureTop),
            };

            for (auto tile : coordinates) {
                if (tile == entity) continue;
                if (!tile) continue;
                if (!(tile->get<SolidComponent>())) continue;
                switch (checkCollisionX(tile, transform, kinetic)) {
                    case Direction::LEFT:
                        kinetic->accX = std::max(0.0f, kinetic->accX);
                        kinetic->speedX = std::max(0.0f, kinetic->speedX);
                        entity->assign<LeftCollisionComponent>();
                        break;
                    case Direction::RIGHT:
                        kinetic->accX = std::min(0.0f, kinetic->accX);
                        kinetic->speedX = std::min(0.0f, kinetic->speedX);
                        entity->assign<RightCollisionComponent>();
                        break;
                    default:
                        break;
                }
            }
        });
    }

    // Apply Forces
    world->find<Engine::TransformComponent, KineticComponent>([&](Entity* entity) {
        if (entity->has<FrozenComponent>()) return;
        auto transform = entity->get<Engine::TransformComponent>();
        auto kinematic = entity->get<KineticComponent>();

        transform->x += kinematic->speedX;
        transform->y += kinematic->speedY;
        kinematic->speedX += kinematic->accX;
        kinematic->speedY += kinematic->accY;

        if (std::abs(kinematic->speedY) < MARIO_ACCELERATION_X) kinematic->speedY = 0;
        if (std::abs(kinematic->speedX) < MARIO_ACCELERATION_X) kinematic->speedX = 0;
        kinematic->speedY *= FRICTION;
        kinematic->speedX *= FRICTION;

        if (kinematic->speedY > MAX_SPEED_Y) kinematic->speedY = MAX_SPEED_Y;
        if (kinematic->speedX > MAX_SPEED_X) kinematic->speedX = MAX_SPEED_X;

        if (kinematic->speedY < -MAX_SPEED_Y) kinematic->speedY = -MAX_SPEED_Y;
        if (kinematic->speedX < -MAX_SPEED_X) kinematic->speedX = -MAX_SPEED_X;
    });
}