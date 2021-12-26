#pragma once

#include "utilities.h"
#include "Players.h"

// namespace for foul detection functions
namespace referee
{
	bool isTurnValid(Player& turnPlayer, TurnInformation& turn);
};