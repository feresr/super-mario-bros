#include "systems/FlagSystem.h"

#include "Components.h"
#include "Constants.h"
#include "AABB.h"

void FlagSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);
}

enum WinAnimationState {
    NO_ANIMATION,
    TAKING_FLAG_DOWN,
    WALKING_TO_CASTLE,
};

WinAnimationState state = NO_ANIMATION;

using namespace Engine;
void FlagSystem::tick(World* world) {

    switch (state) {
        case NO_ANIMATION: {
            auto transform = world->findFirst<PlayerComponent>()->get<TransformComponent>();
            auto tile = world->findFirst<TileMapComponent>()->get<TileMapComponent>()->get(
                    transform->getCenterX() / TILE_SIZE, transform->getCenterY() / TILE_SIZE);
            if (tile && tile->has<Engine::TextureComponent>() && tile->get<TextureComponent>()->id == TextureId::FLAG_POLE) {
                state = TAKING_FLAG_DOWN;
                world->create()->assign<Engine::MusicComponent>(Music::Id::LEVEL_END);
                auto player = world->findFirst<PlayerComponent>();
                player->get<TextureComponent>()->offSetX = -3;
                player->get<TextureComponent>()->flipH = false;
                player->assign<AnimationComponent>(std::vector{TextureId::MARIO_FLAG_2,
                                                               TextureId::MARIO_FLAG_1
                }, 8);
                player->assign<FrozenComponent>();
                return;
            }
            break;
        }
        case TAKING_FLAG_DOWN: {
            auto player = world->findFirst<PlayerComponent>();
            auto transform = player->get<TransformComponent>();
            int flagY = 0;
            transform->h = TILE_SIZE;
            for (auto flag : world->find<FlagComponent>()) {
                flag->get<TransformComponent>()->y += 1.0f;
                flagY = flag->get<TransformComponent>()->y;
            }
            auto tile = world->findFirst<TileMapComponent>()
                    ->get<TileMapComponent>()
                    ->get(transform->right() / TILE_SIZE, transform->bottom() / TILE_SIZE);

            if (tile && tile->get<TextureComponent>()->id != TextureId::FLAG_POLE) {
                if (flagY >= transform->top()) {
                    player->assign<GravityComponent>();
                    player->assign<AnimationComponent>(
                            std::vector<TextureId>{
                                    TextureId::MARIO_RUN_1,
                                    TextureId::MARIO_RUN_2,
                                    TextureId::MARIO_RUN_3},
                            6
                    );
                    player->remove<FrozenComponent>();
                    state = WALKING_TO_CASTLE;
                }
            } else {
                player->get<TransformComponent>()->y += 1.0f;
            }
        }
            break;
        case WALKING_TO_CASTLE : {
            auto player = world->findFirst<PlayerComponent>();
            auto transform = player->get<TransformComponent>();
            player->get<KineticComponent>()->accX = MARIO_ACCELERATION_X;

            auto tile = world->findFirst<TileMapComponent>()
                    ->get<TileMapComponent>()
                    ->get(transform->getCenterX() / TILE_SIZE, transform->getCenterY() / TILE_SIZE);

            if (tile && tile->get<TextureComponent>()->id == TextureId::CASTLE_DOOR) {
                player->assign<FrozenComponent>();
                player->get<TextureComponent>()->id = TextureId::EMPTY;
                player->remove<AnimationComponent>();
                state = NO_ANIMATION;
                player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 280);
            }
        }
            break;
    }
}

void FlagSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void FlagSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
}

FlagSystem::FlagSystem(std::function<void(void)> gameOverCallback) : gameOverCallback{std::move(gameOverCallback)} {

}
