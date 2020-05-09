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

    // Y AXIS CHECK
    if (AABBCollision(
            transform->x + TILE_ROUNDNESS,    // Check previous x position
            transform->y + kinetic->speedY,
            transform->w - (TILE_ROUNDNESS * 2),
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
        } else {
            transform->setTop(solidTransform->bottom());
            solid->assign<BottomCollisionComponent>();
            kinetic->accY = std::max(0.0f, kinetic->accY);
            kinetic->speedY = std::max(0.0f, kinetic->speedY);
            direction = Direction::TOP;
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
            transform->h - 4,
            solidTransform)) {

        float distanceLeft = abs((transform->left() + kinetic->speedX) - solidTransform->right());
        float distanceRight = abs((transform->right() + kinetic->speedX) - solidTransform->left());
        if (distanceLeft < distanceRight) {
            if (transform->left() < solidTransform->right()) {
                //Mario is inside block, push out
                transform->x += std::min(.5f, solidTransform->right() - transform->left());
            } else {
                transform->setLeft(solidTransform->right());
                solid->assign<LeftCollisionComponent>();
                direction = Direction::RIGHT;
            }
            kinetic->accX = std::max(0.0f, kinetic->accX);
            kinetic->speedX = std::max(0.0f, kinetic->speedX);
        } else {
            if (transform->right() > solidTransform->left()) {
                //Mario is inside block, push out
                transform->x -= std::min(.5f, transform->right() - solidTransform->left());
            } else {
                transform->setRight(solidTransform->left());
                solid->assign<RightCollisionComponent>();
                direction = Direction::LEFT;
            }
            kinetic->accX = std::min(0.0f, kinetic->accX);
            kinetic->speedX = std::min(0.0f, kinetic->speedX);
        }
    }
    return direction;
}

void PhysicsSystem::tick(World* world) {
    std::vector<Entity*> entities;
    entities = world->find<GravityComponent, KineticComponent>();
    for (auto entity : entities) entity->get<KineticComponent>()->accY += GRAVITY;

    entities = world->find<WalkComponent, KineticComponent>();
    for (auto entity : entities) {
        if (entity->hasAny<LeftCollisionComponent, RightCollisionComponent>()) {
            entity->get<WalkComponent>()->speed *= -1;
            entity->remove<LeftCollisionComponent>();
            entity->remove<RightCollisionComponent>();
        }

        entity->get<KineticComponent>()->speedX = entity->get<WalkComponent>()->speed;
    }

    for (auto entity : world->find<BreakableComponent, BottomCollisionComponent>()) {
        auto breakable = entity->get<BreakableComponent>();
        if (!breakable->finished()) {
            entity->get<TransformComponent>()->y += (float) breakable->getHeight();
        } else {
            entity->remove<BottomCollisionComponent>();
            breakable->reset();
        }
    }

    // Kinetic-Kinetic collisions
    entities = world->find<TransformComponent, KineticComponent>();
    for (auto entity : entities) {
        if (!entity->has<SolidComponent>()) continue;
        auto transform = entity->get<TransformComponent>();
        auto kinetic = entity->get<KineticComponent>();
        for (auto other : entities) {
            if (entity == other) continue;
            if (!other->has<SolidComponent>()) continue;
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
    auto tileSetEntity = world->findFirst<TileMapComponent>();
    if (tileSetEntity) {
        auto tileSetComponent = tileSetEntity->get<TileMapComponent>();
        auto kineticEntities = world->find<KineticComponent, TransformComponent, SolidComponent>();
        for (auto entity : kineticEntities) {
            auto transform = entity->get<TransformComponent>();
            auto kinetic = entity->get<KineticComponent>();

            int futureLeft = (int) (transform->left() + kinetic->speedX) / TILE_SIZE;
            int futureRight = (int) (transform->right() + kinetic->speedX) / TILE_SIZE;
            int futureTop = (int) (transform->top() + kinetic->speedY) / TILE_SIZE;
            int futureBottom = (int) (transform->bottom() + kinetic->speedY) / TILE_SIZE;
            std::vector<Entity*> coordinates{
                    tileSetComponent->get(futureRight, futureBottom),
                    tileSetComponent->get(futureLeft, futureBottom),
                    tileSetComponent->get(futureLeft, futureTop),
                    tileSetComponent->get(futureRight, futureTop),
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
        auto transform = entity->get<TransformComponent>();
        auto kinematic = entity->get<KineticComponent>();

        transform->x += kinematic->speedX;
        transform->y += kinematic->speedY;
        kinematic->speedX += kinematic->accX;
        kinematic->speedY += kinematic->accY;

        if (std::abs(kinematic->speedY) < .1) kinematic->speedY = 0;
        if (std::abs(kinematic->speedX) < .1) kinematic->speedX = 0;
        kinematic->speedY *= FRICTION;
        kinematic->speedX *= FRICTION;

        if (kinematic->speedY > MAX_SPEED_Y) kinematic->speedY = MAX_SPEED_Y;
        if (kinematic->speedX > MAX_SPEED_X) kinematic->speedX = MAX_SPEED_X;

        if (kinematic->speedY < -MAX_SPEED_Y) kinematic->speedY = -MAX_SPEED_Y;
        if (kinematic->speedX < -MAX_SPEED_X) kinematic->speedX = -MAX_SPEED_X;
    }
}