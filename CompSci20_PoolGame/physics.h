#pragma once

#include "Ball.h"
#include "utilities.h"

namespace physics
{
    void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2);
    void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2);
    void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundaries);
}

struct Rectangle;