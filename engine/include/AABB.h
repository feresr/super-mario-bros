#pragma once

#include "Components.h"

bool AABBCollision(
        Engine::TransformComponent* a,
        Engine::TransformComponent* b
);

bool AABBCollision(
        float x, float y, float w, float h,
        Engine::TransformComponent* b
);