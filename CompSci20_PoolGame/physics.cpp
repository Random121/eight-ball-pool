#include "physics.h"

#include "Ball.h"
#include "common.h"
#include "constants.h"
#include "Vector2.h"
#include "Players.h"

#include <allegro5/allegro_audio.h>

#include <iostream>
#include <vector>
#include <cstddef>

/*
	--IMPORTANT REMINDER FOR COLLISION TESTING--
	We MUST check every ball against every other ball for collision.
	It would be great to not have duplicate checks but an unchecked
	ball could move into another ball. This unchecked collision will
	only be detected once the checked ball has moved from its
	stationary location and will lead to incorrect position and
	velocity resolutions where they either go through each other or
	continuously swap positions in place.

	TLDR; just don't optimize this section.

	I spent like 2 days debugging this mess...
*/

namespace physics
{
	void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2)
	{
		const Vector2 deltaPosition{ ball1.getPositionVector().copyAndSubtract(ball2.getPositionVector()) };

		// calculate distance balls should move by to stop overlapping
		const double ballOverlap{ (deltaPosition.getLength() - ball1.getRadius() - ball2.getRadius()) / 2.0 };

		// change the distance into a vector by making it based on the normal
		const Vector2 moveVector{ deltaPosition.getNormalized().copyAndMultiply(ballOverlap) };

#ifdef DEBUG
		std::cout << "[POSITION RESOLUTION]\n";
		std::cout << "[distance] " << deltaPosition.getLength() << '\n';
		std::cout << "[overlap] " << ballOverlap << '\n';
		std::cout << "[moveDistanceX] " << moveVector.getX() << '\n';
		std::cout << "[moveDistanceY] " << moveVector.getY() << '\n';
		std::cout << "[start ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
		std::cout << "[start ball2] " << ball2.getX() << ", " << ball2.getY() << '\n';
#endif // DEBUG

		ball1.subPosition(moveVector);
		ball2.addPosition(moveVector);

#ifdef DEBUG
		std::cout << "[end ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
		std::cout << "[end ball2] " << ball2.getX() << ", " << ball2.getY() << "\n\n";
#endif // DEBUG
	}

	void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2)
	{
		const Vector2 deltaPosition{ ball1.getPositionVector().copyAndSubtract(ball2.getPositionVector()) };
		const Vector2 deltaVelocity{ ball1.getVelocityVector().copyAndSubtract(ball2.getVelocityVector()) };

		const Vector2 normalVector{ deltaPosition.getNormalized() };

		// solve conservation of momentum
		const double momentum{ 2.0 * normalVector.getDotProduct(deltaVelocity) / (ball1.getMass() + ball2.getMass()) };
		const Vector2 newVelocityVector{ normalVector.copyAndMultiply(momentum * consts::collisionFriction) };

#ifdef DEBUG
		std::cout << "[VELOCITY RESOLUTION]\n";
		std::cout << "[distance] " << deltaPosition.getLength() << '\n';
		std::cout << "[normal] " << normalVector.getX() << ", " << normalVector.getY() << '\n';
		std::cout << "[deltaVelocity] " << deltaVelocity.getX() << ", " << deltaVelocity.getY() << '\n';
		std::cout << "[newVelocity] " << newVelocityVector.getX() << ", " << newVelocityVector.getY() << '\n';
		std::cout << "[start ball1] " << ball1.getVX() << ", " << ball1.getVY() << '\n';
		std::cout << "[start ball2] " << ball2.getVX() << ", " << ball2.getVY() << '\n';
#endif // DEBUG

		ball1.subVelocity(newVelocityVector.copyAndMultiply(ball2.getMass()));
		ball2.addVelocity(newVelocityVector.copyAndMultiply(ball1.getMass()));

#ifdef DEBUG
		std::cout << "[end ball1] " << ball1.getVX() << ", " << ball1.getVY() << '\n';
		std::cout << "[end ball2] " << ball2.getVX() << ", " << ball2.getVY() << "\n\n";
#endif // DEBUG
	}

	bool isCircleCollidingWithBoundaryTop(const Ball& ball, const Rectangle& boundary)
	{
		return (ball.getY() - ball.getRadius()) < boundary.yPos1;
	}

	bool isCircleCollidingWithBoundaryBottom(const Ball& ball, const Rectangle& boundary)
	{
		return (ball.getY() + ball.getRadius()) > boundary.yPos2;
	}

	bool isCircleCollidingWithBoundaryLeft(const Ball& ball, const Rectangle& boundary)
	{
		return (ball.getX() - ball.getRadius()) < boundary.xPos1;
	}

	bool isCircleCollidingWithBoundaryRight(const Ball& ball, const Rectangle& boundary)
	{
		return (ball.getX() + ball.getRadius()) > boundary.xPos2;
	}

	void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundary, bool& didCollide)
	{
		double xPositionAdjustment{};
		double yPositionAdjustment{};

		// check for boundaries in x-axis
		if (isCircleCollidingWithBoundaryLeft(ball, boundary))
		{
			xPositionAdjustment = (boundary.xPos1 - (ball.getX() - ball.getRadius()));
		}
		else if (isCircleCollidingWithBoundaryRight(ball, boundary))
		{
			xPositionAdjustment = -(ball.getX() + ball.getRadius() - boundary.xPos2);
		}

		// check for boundaries in y-axis
		if (isCircleCollidingWithBoundaryTop(ball, boundary))
		{
			yPositionAdjustment = (boundary.yPos1 - (ball.getY() - ball.getRadius()));
		}
		else if (isCircleCollidingWithBoundaryBottom(ball, boundary))
		{
			yPositionAdjustment = -(ball.getY() + ball.getRadius() - boundary.yPos2);
		}

		if (xPositionAdjustment != 0)
		{
			ball.addPosition(xPositionAdjustment, 0);
			ball.setVelocity(-ball.getVX() * consts::collisionFriction, ball.getVY());
			didCollide = true;
		}

		if (yPositionAdjustment != 0)
		{
			ball.addPosition(0, yPositionAdjustment);
			ball.setVelocity(ball.getVX(), -ball.getVY() * consts::collisionFriction);
			didCollide = true;
		}
	}

	// returns true if a collision has happend
	//bool resolveCircleCollisions(Ball& ball, std::vector<Ball>& toBeChecked)
	//{
	//	bool hasCollided{};
	//	for (Ball& checkTarget : toBeChecked)
	//	{
	//		if (ball.isOverlappingBall(checkTarget))
	//		{
	//			resolveCircleCollisionPosition(ball, checkTarget);
	//			resolveCircleCollisionVelocity(ball, checkTarget);
	//			hasCollided = true;
	//		}
	//	}
	//	return hasCollided;
	//}

	bool areBallsMoving(const std::vector<Ball>& gameBalls)
	{
		for (const Ball& ball : gameBalls)
		{
			if (ball.isVisible() && ball.isMoving())
			{
				return true;
			}
		}
		return false;
	}

	// velocity is split into steps as balls can
	// travel too fast and skip collision checks
	void stepPhysics(std::vector<Ball>& gameBalls, Players& gamePlayers, TurnInformation& turn, const AllegroHandler& allegro)
	{
		for (Ball& ball : gameBalls)
		{
			if (!ball.isVisible()) // skip if not visible
				continue;

			const double velocitySum{ std::abs(ball.getVX()) + std::abs(ball.getVY()) };
			double stepsNeeded{ std::ceil(velocitySum / ball.getRadius()) };

			if (stepsNeeded > 0.0)
			{
				const double stepSizeX{ ball.getVX() / stepsNeeded };
				const double stepSizeY{ ball.getVY() / stepsNeeded };

#ifdef DEBUG
				std::cout << "[BALL STEP MOVE " << &ball << "] "
					<< stepSizeX << ", " << stepSizeY << ", " << stepsNeeded << ", "
					<< ball.getX() << ", " << ball.getY() << '\n';
#endif // DEBUG

				bool hasCollided{};
				while (stepsNeeded > 0.0 && !hasCollided)
				{
					ball.addPosition(stepSizeX, stepSizeY);

					// check circle to circle collision
					for (Ball& checkTarget : gameBalls)
					{
						if (ball.isOverlappingBall(checkTarget))
						{
							// calculates loudness of sound depending
							// on the total collision speed
							double collisionLoudness{
								(ball.getVelocityVector().getLength() + checkTarget.getVelocityVector().getLength()) / 50
							};

							// limit volume so it doesn't destroy the user's ears
							if (collisionLoudness > 1.0)
								collisionLoudness = 1.0;

#ifdef DEBUG
							std::cout << "[SOUND LOUDNESS]: " << collisionLoudness << '\n';
#endif // DEBUG

							// if sound is really really quiet then
							// we shouldn't spend resources playing it
							if (collisionLoudness > 0.005)
								al_play_sample(allegro.getAudioSample(AudioSamples::ball_clack), 0.75 * collisionLoudness, 0, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);

							resolveCircleCollisionPosition(ball, checkTarget);
							resolveCircleCollisionVelocity(ball, checkTarget);

							if (turn.firstHitBallType == BallType::unknown)
							{
								// assume the first collision always is cue ball + random ball
								turn.firstHitBallType = (ball.getBallNumber() == 0) ? checkTarget.getBallType() : ball.getBallType();
								turn.didFoulNoRail = true;
							}

							hasCollided = true; // if a collision occured, then stop moving using old velocity
						}
					}

					--stepsNeeded;
				}

				// checks and logic for pocketing
				if (ball.isInPocket())
				{
					// hide and reset ball
					ball.setVisible(false);
					ball.setVelocity(0, 0);

					// update turn informations
					turn.pocketedBalls.push_back(&ball);
					turn.didFoulNoRail = false;

					// check if this is the first pocketed ball, if it is then we
					// set the target suits of each player
					if (gamePlayers.getCurrentPlayer().targetBallType == BallType::unknown)
					{
						if (ball.getBallType() == BallType::solid || ball.getBallType() == BallType::striped)
						{
							gamePlayers.getCurrentPlayer().targetBallType = ball.getBallType();
							gamePlayers.getNextPlayer().targetBallType = (ball.getBallType() == BallType::solid) ? BallType::striped : BallType::solid;
							turn.isTargetBallsSelectedThisTurn = true;
						}
					}

					// play pocket sound
					al_play_sample(allegro.getAudioSample(AudioSamples::ball_pocket), 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);
				}

				ball.applyFriction(consts::rollingFriction, consts::stoppingVelocity);

				bool didCollide{};

				resolveCircleBoundaryCollision(ball, consts::playSurface, didCollide);

				if (didCollide)
				{
					turn.didFoulNoRail = false;
				}
			}
		}
	}
} // namespace physics
