#pragma once

#include "Components.h"

bool AABBCollision(
        TransformComponent* a,
        TransformComponent* b
);

bool AABBCollision(
        float x, float y, float w, float h,
        TransformComponent* b
);