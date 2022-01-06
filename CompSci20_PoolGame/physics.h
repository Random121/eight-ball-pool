#pragma once

#include "AllegroHandler.h"
#include "Ball.h"
#include "Players.h"
#include "common.h"

namespace physics
{
	// circle to circle collision resolution
	void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2);
	void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2);

	// boundary checks
	bool isCircleCollidingWithBoundaryTop(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryBottom(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryLeft(const Ball& ball, const Rectangle& boundary);
	bool isCircleCollidingWithBoundaryRight(const Ball& ball, const Rectangle& boundary);
	void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundary, bool& didCollide);

	// misc important function
	bool areBallsMoving(const Ball::balls_type& gameBalls);
	void stepPhysics(Ball::balls_type& gameBalls, Players& gamePlayers, TurnInformation& turn, const AllegroHandler& allegro);
	//void stepPhysics(Ball::balls_type& gameBalls, Players& gamePlayers, TurnInformation& turn);
	//bool resolveCircleCollisions(Ball& ball, Ball::balls_type& toBeChecked);
}
