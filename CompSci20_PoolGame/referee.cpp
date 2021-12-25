#include "referee.h"

#include "Player.h"
#include "utilities.h"

#include <vector>

namespace referee
{
	// check if the ball first hit by the cue is valid
	bool isValidFirstHit(Player& currentPlayer, BallType hitBallType)
	{
		// did not hit a ball at all
		if (hitBallType == BallType::unknown)
			return false;

		// if the player has not pocketed a ball, it is a foul
		// to hit the eight ball first
		if (currentPlayer.getTargetBallType() == BallType::unknown)
			return hitBallType != BallType::eight;

		// valid since they hit a ball of their type
		if (currentPlayer.getTargetBallType() == hitBallType)
			return true;
	
		// if player pocketed all their balls, they
		// must hit the eight ball first
		if (currentPlayer.getGameScore() == 7)
			return hitBallType == BallType::eight;

		return false;
	}

	bool isPocketedBallsValid(Player& currentPlayer, std::vector<Ball*> pocketedBalls)
	{
		// can't get a foul if player did not pocket anything
		if (pocketedBalls.empty())
			return true;

		for (Ball* ball : pocketedBalls)
		{
			// can't pocket the cue ball
			if (ball->getBallType() == BallType::cue)
				return false;
		}

		if (currentPlayer.getGameScore() == 7)
			return pocketedBalls.size() == 1 && pocketedBalls[0]->getBallType() == BallType::eight;

		return true;
	}

	bool isTurnValid(Player& turnPlayer, TurnInformation& turn)
	{
		return isValidFirstHit(turnPlayer, turn.firstHitBallType) && isPocketedBallsValid(turnPlayer, turn.pocketedBalls);
	}
};