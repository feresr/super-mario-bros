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