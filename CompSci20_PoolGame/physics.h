#pragma once

#include "Ball.h"
#include "utilities.h"

#include <vector>

namespace physics
{
    void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2);
    void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2);
    void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundaries);
    void advanceBallPosition(Ball& ball, const double friction, const double stopVelocity, const double deltaTime);
    void stepPhysics(std::vector<Ball>& vecBalls, const double deltaTime);
}