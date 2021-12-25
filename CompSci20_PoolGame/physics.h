#pragma once

#include "Ball.h"
#include "Player.h"
#include "utilities.h"

#include <vector>

namespace physics
{
	void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2);
	void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2);
	bool isCircleCollidingWithBoundaryTop(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryBottom(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryLeft(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryRight(const Ball& ball, const Rectangle& boundary);
	void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundaries);
	bool resolveCircleCollisions(Ball& ball, std::vector<Ball>& toBeChecked);
	bool areBallsMoving(const std::vector<Ball>& gameBalls);
	void stepPhysics(std::vector<Ball>& gameBalls, std::vector<Player>& gamePlayers, TurnInformation& turn);
}