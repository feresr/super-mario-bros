#include "systems/PhysicsSystem.h"

float dirX = 0;
bool jump = false;
float left = 0;
float right = 0;

void PhysicsSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

void PhysicsSystem::tick(World* world) {
    std::vector<Entity*> entities;
    entities = world->find<GravityComponent, KineticComponent>();
    for (auto entity : entities) entity->get<KineticComponent>()->accY += GRAVITY;

    auto player = world->findFirst<PlayerComponent, KineticComponent>();
    player->get<KineticComponent>()->accX = dirX * MARIO_ACCELERATION_X;
    if (jump) {
        player->get<KineticComponent>()->speedY = -MARIO_JUMP;
        jump = false;
    }

    entities = world->find<KineticComponent>();
    for (auto entity : entities) {
        auto transform = entity->get<TransformComponent>();
        auto kinetic = entity->get<KineticComponent>();
        std::vector<Entity*> solids = world->find<SolidComponent>();
        for (auto solid : solids) {
            auto solidTransform = solid->get<TransformComponent>();
            if (solid == entity) continue;
            //Moving down
            if (transform->top() < solidTransform->top() &&
                transform->left() + TILE_ROUNDNESS < solidTransform->right() &&
                transform->right() - TILE_ROUNDNESS> solidTransform->left() &&
                transform->bottom() + kinetic->speedY > solidTransform->top()) {
                kinetic->speedY = 0;
                kinetic->accY = 0;
                transform->setBottom(solidTransform->y);
            }
            //Moving up
            if (transform->bottom() > solidTransform->bottom() &&
                transform->left() + TILE_ROUNDNESS < solidTransform->right() &&
                transform->right() - TILE_ROUNDNESS > solidTransform->left() &&
                transform->top() + kinetic->speedY < solidTransform->bottom()) {
                kinetic->speedY = 0;
                kinetic->accY = 0;
                transform->setTop(solidTransform->bottom());
            }

            //if (kinetic->speedX < 0) {
            //Moving left
            if (transform->right() > solidTransform->right() &&
                transform->top() < solidTransform->bottom() &&
                transform->bottom() > solidTransform->top() &&
                transform->left() + kinetic->speedX < solidTransform->right()) {
                kinetic->speedX = std::max(0.0f, kinetic->speedX);
                kinetic->accX = 0;
                transform->setLeft(solidTransform->right());
            }
            //}
            //if (kinetic->speedX > 0) {
            //Moving right
            if (transform->left() < solidTransform->left() &&
                transform->top() < solidTransform->bottom() &&
                transform->bottom() > solidTransform->top() &&
                transform->right() + kinetic->speedX > solidTransform->left()) {
                kinetic->speedX = std::min(0.0f, kinetic->speedX);
                kinetic->accX = 0;
                transform->setRight(solidTransform->left());
            }
            //}
        }
    }

    entities = world->find<TransformComponent, KineticComponent>();
    for (auto entity : entities) {
        auto transform = entity->get<TransformComponent>();
        auto kinematic = entity->get<KineticComponent>();
        kinematic->speedY *= FRICTION;
        kinematic->speedX *= FRICTION;
        if (std::abs(kinematic->speedY) < .01) kinematic->speedY = 0;
        if (std::abs(kinematic->speedX) < .01) kinematic->speedX = 0;
        kinematic->speedX += kinematic->accX;
        kinematic->speedY += kinematic->accY;

        transform->x += (int) kinematic->speedX;
        transform->y += (int) kinematic->speedY;
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


