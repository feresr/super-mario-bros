#include "systems/PhysicsSystem.h"

bool AABBCollision(
        float x, float y, float w, float h,
        TransformComponent* b
) {
    return x <= b->x + b->w &&
           x + w >= b->x &&
           y <= b->y + b->h &&
           y + h >= b->y;
}

Direction checkCollisionY(Entity* solid, TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<TransformComponent>();
    auto direction = Direction::NONE;

    if (kinetic->speedY >= 0.0f) {
        // Falling
        if (AABBCollision(
                transform->x + TILE_ROUNDNESS / 2,    // Check previous x position
                transform->y + kinetic->speedY,
                transform->w - (TILE_ROUNDNESS),
                transform->h,
                solidTransform)) {
            float distanceTop = abs(solidTransform->top() - (transform->bottom() + kinetic->speedY));
            float distanceBottom = abs((transform->top() + kinetic->speedY) - solidTransform->bottom());
            if (distanceTop < distanceBottom) {
                transform->setBottom(solidTransform->top());
                solid->assign<TopCollisionComponent>();
                kinetic->accY = std::min(0.0f, kinetic->accY);
                kinetic->speedY = std::min(0.0f, kinetic->speedY);
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
                kinetic->accY = std::max(0.0f, kinetic->accY);
                kinetic->speedY = std::max(0.0f, kinetic->speedY);
                direction = Direction::TOP;
            }
        }
    }
    return direction;
}

Direction checkCollisionX(Entity* solid, TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<TransformComponent>();
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
            kinetic->accX = std::max(0.0f, kinetic->accX);
            kinetic->speedX = std::max(0.0f, kinetic->speedX);
            solid->assign<RightCollisionComponent>();
            direction = Direction::LEFT;
        } else {
            if (transform->right() > solidTransform->left()) {
                //item is inside block, push out
                transform->x -= std::min(.5f, transform->right() - solidTransform->left());
            } else {
                transform->setRight(solidTransform->left());
            }
            kinetic->accX = std::min(0.0f, kinetic->accX);
            kinetic->speedX = std::min(0.0f, kinetic->speedX);
            solid->assign<LeftCollisionComponent>();
            direction = Direction::RIGHT;
        }
    }
    return direction;
}

void PhysicsSystem::tick(World* world) {
    std::vector<Entity*> entities;
    entities = world->find<GravityComponent, KineticComponent>();
    for (auto entity : entities) entity->get<KineticComponent>()->accY += GRAVITY;

    // Kinetic-Kinetic collisions
    entities = world->find<TransformComponent, KineticComponent>();
    for (auto entity : entities) {
        if (!entity->has<SolidComponent>()) continue;
        auto transform = entity->get<TransformComponent>();
        auto kinetic = entity->get<KineticComponent>();
        for (auto other : entities) {
            if (entity == other) continue;
            if (!other->has<SolidComponent>()) continue;
            if (other->has<CollectibleComponent>()) continue;
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
        }
    }

    // Check Kinetic-Tiles Collisions
    auto tileMapEntity = world->findFirst<TileMapComponent>();
    if (tileMapEntity) {
        auto tileMapComponent = tileMapEntity->get<TileMapComponent>();
        auto kineticEntities = world->find<KineticComponent, TransformComponent, SolidComponent>();
        for (auto entity : kineticEntities) {
            auto transform = entity->get<TransformComponent>();
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
                if (!tile) continue;
                if (!(tile->get<SolidComponent>())) continue;
                switch (checkCollisionY(tile, transform, kinetic)) {
                    case Direction::TOP:
                        entity->assign<TopCollisionComponent>();
                        break;
                    case Direction::BOTTOM:
                        entity->assign<BottomCollisionComponent>();
                        break;
                    default:
                        break;
                }
            }
            for (auto tile : coordinates) {
                if (!tile) continue;
                if (!(tile->get<SolidComponent>())) continue;
                switch (checkCollisionX(tile, transform, kinetic)) {
                    case Direction::LEFT:
                        entity->assign<LeftCollisionComponent>();
                        break;
                    case Direction::RIGHT:
                        entity->assign<RightCollisionComponent>();
                        break;
                    default:
                        break;
                }
            }
        }
    }

    // Apply Forces
    entities = world->find<TransformComponent, KineticComponent>();
    for (auto entity : entities) {
        if (entity->has<FrozenComponent>()) continue;
        auto transform = entity->get<TransformComponent>();
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
    }
}