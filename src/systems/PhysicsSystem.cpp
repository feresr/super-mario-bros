#include "systems/PhysicsSystem.h"

float dirX = 0;
bool jump = false;
float left = 0;
float right = 0;

void PhysicsSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

Direction checkCollision(Entity* solid, TransformComponent* transform, KineticComponent* kinetic) {
    auto solidTransform = solid->get<TransformComponent>();

    //Moving down
    if (transform->top() < solidTransform->top() &&
        transform->left() + TILE_ROUNDNESS < solidTransform->right() &&
        transform->right() - TILE_ROUNDNESS > solidTransform->left() &&
        transform->bottom() + kinetic->speedY > solidTransform->top()) {
        kinetic->speedY = 0;
        kinetic->accY = 0;
        transform->setBottom(solidTransform->top());
        return Direction::BOTTOM;
    }

    //Moving up
    if (transform->bottom() > solidTransform->bottom() &&
        transform->left() + TILE_ROUNDNESS < solidTransform->right() &&
        transform->right() - TILE_ROUNDNESS > solidTransform->left() &&
        transform->top() + kinetic->speedY < solidTransform->bottom()) {
        kinetic->speedY = 0;
        kinetic->accY = 0;
        transform->setTop(solidTransform->bottom());
        solid->assign<BottomCollisionComponent>();
        return Direction::TOP;
    }

    //Moving left
    if (transform->right() > solidTransform->right() &&
        transform->top() < solidTransform->bottom() &&
        transform->bottom() > solidTransform->top() &&
        transform->left() + kinetic->speedX < solidTransform->right()) {
        if (kinetic->speedX < 0) {
            kinetic->speedX = std::max(0.0f, kinetic->speedX);
            kinetic->accX = 0;
            transform->setLeft(solidTransform->right());
            return Direction::LEFT;
        } else {
            transform->x += .5;
        }
    }

    //Moving right
    if (transform->left() < solidTransform->left() &&
        transform->top() < solidTransform->bottom() &&
        transform->bottom() > solidTransform->top() &&
        transform->right() + kinetic->speedX > solidTransform->left()) {
        if (kinetic->speedX > 0) {
            kinetic->speedX = std::min(0.0f, kinetic->speedX);
            kinetic->accX = 0;
            transform->setRight(solidTransform->left());
            return Direction::RIGHT;
        } else {
            transform->x -= .5;
        }
    }

    return Direction::NONE;
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

    auto player = world->findFirst<PlayerComponent, KineticComponent>();
    if (player) { // TODO: REMOVE IF, FORCE A PLAYER TO BE PRESENT
        player->get<KineticComponent>()->accX = dirX * MARIO_ACCELERATION_X;
        if (jump) {
            player->get<KineticComponent>()->accY = -MARIO_JUMP;
            jump = false;
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

    // Apply Forces
    entities = world->find<TransformComponent, KineticComponent>();
    for (auto entity : entities) {
        auto transform = entity->get<TransformComponent>();
        auto kinematic = entity->get<KineticComponent>();
        kinematic->speedX += kinematic->accX;
        kinematic->speedY += kinematic->accY;
        transform->x += kinematic->speedX;
        transform->y += kinematic->speedY;

        // Apply friction
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

    // Check Collisions
    auto tileSetEntity = world->findFirst<TileMapComponent>();
    if (tileSetEntity) {
        auto tileSetComponent = tileSetEntity->get<TileMapComponent>();
        auto kineticEntities = world->find<KineticComponent, TransformComponent>();
        //Collision against tiles
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

    //Collision against enemies?
    for (auto entity : entities) {
        auto transform = entity->get<TransformComponent>();
        auto kinetic = entity->get<KineticComponent>();
        std::vector<Entity*> solids = world->find<SolidComponent>();
        /*for (auto solid : solids) {

        }*/
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


