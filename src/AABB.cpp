#include "AABB.h"


bool AABBCollision(
        TransformComponent* a,
        TransformComponent* b
) {
    return a->x <= b->x + b->w &&
           a->x + a->w >= b->x &&
           a->y <= b->y + b->h &&
           a->y + a->h >= b->y;
}

bool AABBCollision(
        float x, float y, float w, float h,
        TransformComponent* b
) {
    return x <= b->x + b->w &&
           x + w >= b->x &&
           y <= b->y + b->h &&
           y + h >= b->y;
}