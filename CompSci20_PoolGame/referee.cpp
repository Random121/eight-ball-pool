#include "referee.h"

#include "Players.h"
#include "common.h"

#include <iostream>

namespace referee
{
	// check if the ball first hit by the cue is valid
	bool isValidFirstHit(Players::PlayerType& currentPlayer, Ball::BallSuitType hitBallType)
	{
		// did not hit a ball at all
		if (hitBallType == Ball::BallSuitType::unknown)
			return false;

		// if the player has not pocketed a ball, it is a foul
		// to hit the eight ball first
		if (currentPlayer.targetBallType == Ball::BallSuitType::unknown)
			return hitBallType != Ball::BallSuitType::eight;

		// if player pocketed all their balls, they
		// must hit the eight ball first
		if (currentPlayer.score == 7)
			return hitBallType == Ball::BallSuitType::eight;

		// valid since they hit a ball of their type
		if (currentPlayer.score > 0 && currentPlayer.targetBallType != hitBallType)
			return false;

		return true;
	}

	bool isPocketedBallsValid(Players::PlayerType& currentPlayer, Ball::ballsPointer_type pocketedBalls)
	{
		// can't get a foul if player did not pocket anything
		if (pocketedBalls.empty())
			return true;

		for (Ball* ball : pocketedBalls)
		{
			const Ball::BallSuitType& type{ ball->getBallType() };

			// can't pocket cue ball or pocket the eight ball while not finished
			if ((type == Ball::BallSuitType::cue) || (currentPlayer.score != 7 && type == Ball::BallSuitType::eight))
			{
				return false;
			}
		}

		if (currentPlayer.score == 7)
			return pocketedBalls.size() == 1 && pocketedBalls[0]->getBallType() == Ball::BallSuitType::eight;

		return true;
	}

	bool isTurnValid(Players::PlayerType& turnPlayer, const TurnInformation& turn)
	{
		return isValidFirstHit(turnPlayer, turn.firstHitBallType) && isPocketedBallsValid(turnPlayer, turn.pocketedBalls) && !turn.didFoulNoRail;
	}

	bool isGameFinished(const Ball::balls_type& gameBalls)
	{
		return !gameBalls[8].isVisible();
	}
} // namespace referee
