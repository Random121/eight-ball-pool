#include "physics.h"

#include "Ball.h"
#include "common.h"
#include "constants.h"
#include "Vector2.h"
#include "Players.h"

#include <allegro5/allegro_audio.h>

#include <iostream>
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
*/

namespace physics
{
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

	bool areBallsMoving(const Ball::balls_type& gameBalls)
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

	static void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2)
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

	static void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2)
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

	static bool resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundary)
	{
		bool didCollide{};
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

		return didCollide;
	}

	static void playBallCollisionSound(const AllegroHandler& allegro, const Ball& ball1, const Ball& ball2)
	{
		double volume{ (ball1.getVelocityVector().getLength() + ball2.getVelocityVector().getLength()) / 50 };

		// sound is too quiet to play
		if (volume < 0.005)
			return;

		// limit max volume
		if (volume > 1.0)
			volume = 1.0;

#ifdef DEBUG
		std::cout << "[SOUND LOUDNESS]: " << collisionLoudness << '\n';
#endif // DEBUG

		al_play_sample(
			allegro.getAudioSample(AudioSamples::ball_clack), // sound sample
			0.75 * volume, // volume
			0, // balance
			1, // playback speed
			ALLEGRO_PLAYMODE_ONCE,
			nullptr
		);
	}

	static Ball::BallSuitType getOppositeSuit(const Ball& ball)
	{
		return (ball.getBallType() == Ball::BallSuitType::solid)
			? Ball::BallSuitType::striped
			: Ball::BallSuitType::solid;
	}

	static void handlePocketing(Ball& ball, Players& gamePlayers, TurnInformation& currentTurn, const AllegroHandler& allegro)
	{
		if (!ball.isInPocket())
			return;

		// stop ball and mark it as inactive
		ball.setVelocity(0, 0);
		ball.setVisible(false);

		// update turn informations
		currentTurn.pocketedBalls.push_back(&ball);
		currentTurn.didNoRailFoul = false;

		// check if this is the first pocketed ball
		// if it is, we set the target suit of players
		if (gamePlayers.getCurrentPlayer().targetBallType == Ball::BallSuitType::unknown)
		{
			if (ball.isSuitBall())
			{
				gamePlayers.getCurrentPlayer().targetBallType = ball.getBallType();
				gamePlayers.getNextPlayer().targetBallType = getOppositeSuit(ball);
				currentTurn.targetBallsSelectedThisTurn = true;
			}
		}

		// play pocketing sound
		al_play_sample(
			allegro.getAudioSample(AudioSamples::ball_pocket), // sound sample
			0.5, // volume
			0, // balance
			1, // playback speed
			ALLEGRO_PLAYMODE_ONCE,
			nullptr
		);
	}

	// in this function we calculate:
	// - ball movement
	// - ball friction
	// - ball to ball collisions
	// - ball to boundary collisions
	void stepPhysics(Ball::balls_type& gameBalls, Players& gamePlayers, TurnInformation& currentTurn, const AllegroHandler& allegro)
	{
		for (Ball& ball : gameBalls)
		{
			// skip inactive balls
			if (!ball.isVisible())
				continue;

			const double velocitySum{ std::abs(ball.getVX()) + std::abs(ball.getVY()) };
			double stepsNeeded{ std::ceil(velocitySum / ball.getRadius()) };

			if (stepsNeeded > 0.0)
			{
				bool hasCollided{};
				const double stepSizeX{ ball.getVX() / stepsNeeded };
				const double stepSizeY{ ball.getVY() / stepsNeeded };

#ifdef DEBUG
				std::cout << "[BALL STEP MOVE " << &ball << "] "
					<< stepSizeX << ", " << stepSizeY << ", " << stepsNeeded << ", "
					<< ball.getX() << ", " << ball.getY() << '\n';
#endif // DEBUG

				while (stepsNeeded > 0.0 && !hasCollided)
				{
					ball.addPosition(stepSizeX, stepSizeY);

					// check circle to circle collision
					for (Ball& checkTarget : gameBalls)
					{
						if (ball.isOverlappingBall(checkTarget))
						{
							playBallCollisionSound(allegro, ball, checkTarget);

							resolveCircleCollisionPosition(ball, checkTarget);
							resolveCircleCollisionVelocity(ball, checkTarget);

							if (currentTurn.firstHitBallType == Ball::BallSuitType::unknown)
							{
								// assume the first collision always is cue ball + random ball
								currentTurn.firstHitBallType = (ball.getBallNumber() == 0) ? checkTarget.getBallType() : ball.getBallType();
								currentTurn.didNoRailFoul = true;
							}

							// collision has occured, stop moving using old velocity
							hasCollided = true;
						}
					}

					--stepsNeeded;
				}

				handlePocketing(ball, gamePlayers, currentTurn, allegro);

				ball.applyFriction(consts::rollingFriction, consts::stoppingVelocity);

				if (resolveCircleBoundaryCollision(ball, consts::playSurface))
				{
					currentTurn.didNoRailFoul = false;
				}
			}
		}
	}
} // namespace physics
