#include <Engine.h>
#include "systems/ScoreSystem.h"
#include "Components.h"
#include "Constants.h"

void ScoreSystem::onAddedToWorld(World* world) {
    System::onAddedToWorld(world);

    auto paddingH = 22;
    auto paddingV = 16;
    auto textHeight = 8;
    auto spacingV = -0;

    auto availableWidth = SNES_RESOLUTION_WIDTH - paddingH * 2;
    auto columns = 4;
    auto columnWidth = availableWidth / columns;

    //FIRST COLUMN (left aligned)
    auto marioEntity = world->create();
    marioEntity->assign<Engine::TextComponent>("MARIO");
    marioEntity->assign<Engine::TransformComponent>(paddingH, paddingV, 40, textHeight);

    scoreEntity = world->create();
    scoreEntity->assign<Engine::TextComponent>("000000");
    scoreEntity->assign<Engine::TransformComponent>(paddingH, paddingV + textHeight + spacingV, 48, textHeight);

    // SECOND COLUMN (center aligned)
    coinsEntity = world->create();
    coinsEntity->assign<Engine::TextComponent>("x00");
    auto w = 24;
    coinsEntity->assign<Engine::TransformComponent>(paddingH + columnWidth + (columnWidth - w) / 2 + 5,
                                            paddingV + textHeight + spacingV, w, textHeight);
    auto coinIco = world->create();
    coinIco->assign<Engine::TextureComponent>(TextureId::COIN_SMALL_1);
    coinIco->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_1,
                    TextureId::COIN_SMALL_3,
                    TextureId::COIN_SMALL_2,
                    TextureId::COIN_SMALL_3
            }, 10);

    coinIco->assign<Engine::TextComponent>("");
    coinIco->assign<Engine::TransformComponent>(
            paddingH + columnWidth + (columnWidth - w) / 2 - 4,
            paddingV + textHeight + spacingV, 5, 8
    );

    // THIRD COLUMN (center aligned)
    auto worldEntity = world->create();
    worldEntity->assign<Engine::TextComponent>("WORLD");
    w = 40;
    worldEntity->assign<Engine::TransformComponent>(paddingH + columnWidth * 2 + (columnWidth - w) / 2,
                                            paddingV, w, textHeight);

    auto worldNumberEntity = world->create();
    worldNumberEntity->assign<Engine::TextComponent>("1-1");
    w = 24;
    worldNumberEntity->assign<Engine::TransformComponent>(paddingH + columnWidth * 2 + (columnWidth - w) / 2,
                                                  paddingV + textHeight + spacingV, w, textHeight);

    // FOURTH COLUMN (right aligned)
    auto timeEntity = world->create();
    timeEntity->assign<Engine::TextComponent>("TIME");
    timeEntity->assign<Engine::TransformComponent>(SNES_RESOLUTION_WIDTH - paddingH - 32, paddingV, 32, textHeight);

    timeLeftEntity = world->create();
    timeLeftEntity->assign<Engine::TextComponent>("000");
    timeLeftEntity->assign<Engine::TransformComponent>(SNES_RESOLUTION_WIDTH - paddingH - 24,
                                               paddingV + textHeight + spacingV, 24, textHeight);
}

void ScoreSystem::tick(World* world) {
    bool updateScore = false;
    bool updateCoins = false;

    for (auto e : world->find<AddScoreComponent>()) {
        auto s = e->get<AddScoreComponent>();
        score += s->score;
        updateScore = true;
        if (s->coin) {
            coins++;
            updateCoins = true;
        }
        world->destroy(e);
    }

    if (updateScore) {
        auto scoreString = std::to_string(score);
        auto final = std::string{};
        auto zeros = 6 - scoreString.length();
        while (zeros > 0) {
            zeros--;
            final += '0';
        }
        final += scoreString;
        scoreEntity->assign<Engine::TextComponent>(std::move(final));
    }

    if (updateCoins) {
        auto final = std::string{};
        auto coinString = std::to_string(coins);
        auto zeros = 2 - coinString.length();
        while (zeros > 0) {
            zeros--;
            final += '0';
        }
        final += coinString;
        coinsEntity->assign<Engine::TextComponent>("x" + final);
    }

    time--;
    if (time >= 0) {
        auto final = std::string{};
        auto timeString = std::to_string(time / 60);
        auto zeros = 3 - timeString.length();
        while (zeros > 0) {
            zeros--;
            final += '0';
        }
        final += timeString;
        timeLeftEntity->assign<Engine::TextComponent>(std::move(final));
        if (time <= 0) timeLeftEntity->assign<GameOverComponent>();
    }

    for (auto points: world->find<FloatingPointsComponent>()) {
        auto camera = world->findFirst<Engine::CameraComponent>()->get<Engine::CameraComponent>();
        auto pointsComponent = points->get<FloatingPointsComponent>();

        auto pointEntity = world->create();

        TextureId textureId;
        switch (pointsComponent->points) {
            case Points::ONEHOUNDRED:
                textureId = TextureId::ONEHUNDRED;
                break;
            case Points::TWOHOUNDRED:
                textureId = TextureId::TWOHUNDRED;
                break;
            case Points::ONETHOUSAND:
                textureId = TextureId::THOUSAND;
                break;
            case Points::ONEUP:
                textureId = TextureId::ONE_UP_LABEL;
                break;
        }
        pointEntity->assign<Engine::TextureComponent>(textureId);
        pointEntity->assign<Engine::TextComponent>("");
        pointEntity->assign<Engine::TransformComponent>((pointsComponent->x - camera->left()),
                                                pointsComponent->y, 0, 0);
        pointEntity->assign<KineticComponent>(0, 0);
        pointEntity->get<KineticComponent>()->speedY = -2.0f;
        pointEntity->assign<CallbackComponent>([=]() { world->destroy(pointEntity); }, 50);
        world->destroy(points);
    }
}

void ScoreSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void ScoreSystem::onRemovedFromWorld(World* world) {
    System::onRemovedFromWorld(world);
    world->destroy(coinsEntity);
    world->destroy(scoreEntity);
}
