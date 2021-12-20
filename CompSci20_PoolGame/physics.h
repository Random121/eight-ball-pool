#pragma once

#include "Ball.h"
#include "utilities.h"

#include <vector>

namespace physics
{
	void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2);
	void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2);
	void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundaries);
	bool resolveCircleCollisions(Ball& ball, std::vector<Ball>& toBeChecked);
	void stepPhysics(std::vector<Ball>& vecBalls, bool& allBallsStopped);
}