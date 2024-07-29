#pragma once

#include "common.h"
#include "Players.h"

// namespace for foul detection functions
namespace referee
{
	bool isTurnValid(Players::PlayerType& turnPlayer, const TurnInformation& turn);
	bool isGameFinished(const Ball::balls_type& gameBalls);
}
