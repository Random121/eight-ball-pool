#pragma once

#include "common.h"
#include "Players.h"

// namespace for foul detection functions
namespace referee
{
	bool isTurnValid(Player& turnPlayer, const TurnInformation& turn);
	bool isGameFinished(const std::vector<Ball>& gameBalls);
};