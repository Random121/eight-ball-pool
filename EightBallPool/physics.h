#pragma once

#include "AllegroHandler.h"
#include "Ball.h"
#include "Players.h"
#include "common.h"

namespace physics
{
	void stepPhysics(Ball::balls_type& gameBalls, Players& gamePlayers, TurnInformation& turn, const AllegroHandler& allegro);

	// boundary checks
	bool isCircleCollidingWithBoundaryTop(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryBottom(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryLeft(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryRight(const Ball& ball, const Rectangle& boundary);

	// misc function
	bool areBallsMoving(const Ball::balls_type& gameBalls);
}
