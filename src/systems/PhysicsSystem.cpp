#include "systems/PhysicsSystem.h"

float dirX = 0;
bool jump = false;
float left = 0;
float right = 0;

void PhysicsSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

bool AABBCollision(
        float x, float y, float w, float h,
        TransformComponent* b
) {
    return x < b->x + b->w &&
           x + w > b->x &&
           y < b->y + b->h &&
           y + h > b->y;
}

Direction checkCollision(Entity* solid, TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<TransformComponent>();
    auto direction = Direction::NONE;

    // X-AXIS CHECK
    if (AABBCollision(
            transform->x + kinetic->speedX,
            transform->y,   // Check previous y position
            transform->w,
            transform->h,
            solidTransform)) {

        float distanceLeft = abs((transform->left() + kinetic->speedX) - solidTransform->right());
        float distanceRight = abs((transform->right() + kinetic->speedX) - solidTransform->left());
        if (distanceLeft < distanceRight) {
            transform->setLeft(solidTransform->right());
            solid->assign<LeftCollisionComponent>();
            kinetic->accX = std::min(0.0f, kinetic->accX);
            direction = Direction::RIGHT;
        } else {
            transform->setRight(solidTransform->left());
            solid->assign<RightCollisionComponent>();
            kinetic->accX = std::max(0.0f, kinetic->accX);
            direction = Direction::LEFT;
        }
        kinetic->speedX = 0;
    }

    // Y AXIS CHECK
    if (AABBCollision(
            transform->x + kinetic->speedX,    // Check with updated X position
            transform->y + kinetic->speedY,
            transform->w,
            transform->h,
            solidTransform)) {

        float distanceTop = abs(solidTransform->top() - (transform->bottom() + kinetic->speedY));
        float distanceBottom = abs((transform->top() + kinetic->speedY) - solidTransform->bottom());
        if (distanceTop < distanceBottom) {
            transform->setBottom(solidTransform->top());
            solid->assign<TopCollisionComponent>();
            kinetic->accY = std::min(0.0f, kinetic->accY);
            direction = Direction::BOTTOM;
        } else {
            transform->setTop(solidTransform->bottom());
            solid->assign<BottomCollisionComponent>();
            kinetic->accY = std::max(0.0f, kinetic->accY);
            direction = Direction::TOP;
        }
        kinetic->speedY = 0;
    }

    return direction;
}

constexpr std::pair<int, int> TILE_OFFSETS[9] = {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(-1, 0),
        std::make_pair(1, 0),
        std::make_pair(-1, 1),
        std::make_pair(1, 1),
        std::make_pair(1, -1),
        std::make_pair(-1, -1),
        std::make_pair(0, 0),
};

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

    auto player = world->findFirst<PlayerComponent>();
    if (player) { // TODO: REMOVE IF, FORCE A PLAYER TO BE PRESENT
        player->get<KineticComponent>()->accX = dirX * MARIO_ACCELERATION_X;
        if (jump) {
            player->get<KineticComponent>()->accY = -MARIO_JUMP;
            jump = false;
        }

        for (auto enemy : world->find<EnemyComponent>()) {
            if (enemy->has<TopCollisionComponent>()) {
                enemy->remove<WalkComponent>();
                enemy->remove<KineticComponent>();
                enemy->remove<TopCollisionComponent>();
                player->get<KineticComponent>()->accY = -.4f;
            }
        }
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
            switch (checkCollision(other, transform, kinetic)) {
                case Direction::LEFT:
                    entity->assign<LeftCollisionComponent>();
                    break;
                case Direction::RIGHT:
                    entity->assign<RightCollisionComponent>();
                    break;
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
    }

    // Check Kinetic-Tiles Collisions
    auto tileSetEntity = world->findFirst<TileMapComponent>();
    if (tileSetEntity) {
        auto tileSetComponent = tileSetEntity->get<TileMapComponent>();
        auto kineticEntities = world->find<KineticComponent, TransformComponent, SolidComponent>();
        // Collision against tiles
        for (auto entity : kineticEntities) {
            auto transform = entity->get<TransformComponent>();
            auto kinetic = entity->get<KineticComponent>();

            for (auto offset : TILE_OFFSETS) {
                auto x = (transform->getCenterX() / TILE_SIZE) + offset.first;
                auto y = (transform->getCenterY() / TILE_SIZE) + offset.second;
                auto tile = tileSetComponent->get(x, y);
                if (!tile) continue;
                if (!(tile->get<SolidComponent>())) continue;
                switch (checkCollision(tile, transform, kinetic)) {
                    case Direction::LEFT:
                        entity->assign<LeftCollisionComponent>();
                        break;
                    case Direction::RIGHT:
                        entity->assign<RightCollisionComponent>();
                        break;
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

        kinematic->speedY *= FRICTION;
        kinematic->speedX *= FRICTION;
        if (std::abs(kinematic->speedY) < .01) kinematic->speedY = 0;
        if (std::abs(kinematic->speedX) < .01) kinematic->speedX = 0;

        if (kinematic->speedY > MAX_SPEED) kinematic->speedY = MAX_SPEED;
        if (kinematic->speedX > MAX_SPEED) kinematic->speedX = MAX_SPEED;

        if (kinematic->speedY < -MAX_SPEED) kinematic->speedY = -MAX_SPEED;
        if (kinematic->speedX < -MAX_SPEED) kinematic->speedX = -MAX_SPEED;
        // --------------
    }
}

void PhysicsSystem::handleEvent(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_D:
                    right = true;
                    break;
                case SDL_SCANCODE_W:
                    jump = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                    left = false;
                    break;
                case SDL_SCANCODE_D:
                    right = false;
                    break;
                default:
                    break;
            }
    }

    dirX = right - left;
}

void PhysicsSystem::onRemovedFromWorld(World* world) {
}


