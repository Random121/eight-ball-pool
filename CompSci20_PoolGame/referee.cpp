#include "referee.h"

#include "Players.h"
#include "common.h"

#include <vector>
#include <iostream>

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
		if (currentPlayer.targetBallType == BallType::unknown)
			return hitBallType != BallType::eight;

		// if player pocketed all their balls, they
		// must hit the eight ball first
		if (currentPlayer.score == 7)
			return hitBallType == BallType::eight;

		// valid since they hit a ball of their type
		if (currentPlayer.score > 0 && currentPlayer.targetBallType != hitBallType)
			return false;

		return true;
	}

	bool isPocketedBallsValid(Player& currentPlayer, std::vector<Ball*> pocketedBalls)
	{
		// can't get a foul if player did not pocket anything
		if (pocketedBalls.empty())
			return true;

		for (Ball* ball : pocketedBalls)
		{
			const BallType& type{ ball->getBallType() };

			// can't pocket cue ball or pocket the eight ball while not finished
			if ((type == BallType::cue) || (currentPlayer.score != 7 && type == BallType::eight))
			{
				return false;
			}
		}

		if (currentPlayer.score == 7)
			return pocketedBalls.size() == 1 && pocketedBalls[0]->getBallType() == BallType::eight;

		return true;
	}

	bool isTurnValid(Player& turnPlayer, const TurnInformation& turn)
	{
		return isValidFirstHit(turnPlayer, turn.firstHitBallType) && isPocketedBallsValid(turnPlayer, turn.pocketedBalls);
	}

	bool isGameFinished(const std::vector<Ball>& gameBalls)
	{
		return !gameBalls[8].isVisible();
	}
};