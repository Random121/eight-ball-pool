#include "physics.h"

#include "Ball.h"
#include "common.h"
#include "constants.h"
#include "Vector2.h"
#include "Players.h"

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
		std::cout << "[distance] " << ballDistance << '\n';
		std::cout << "[overlap] " << ballOverlap << '\n';
		std::cout << "[moveDistanceX] " << moveDistanceX << '\n';
		std::cout << "[moveDistanceY] " << moveDistanceY << '\n';
		std::cout << "[start ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
		std::cout << "[start ball2] " << ball2.getX() << ", " << ball2.getY() << '\n';0
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
		std::cout << "[distance] " << ballDistance << '\n';
		std::cout << "[normal] " << normalX << ", " << normalY << '\n';
		std::cout << "[deltaVelocity] " << deltaVelocityX << ", " << deltaVelocityY << '\n';
		std::cout << "[newVelocity] " << newVelocityX << ", " << newVelocityY << '\n';
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

	// this function is a mess...
	// we have to do this velocity step stuff as the balls can be too fast
	// and result in skipped collision checks
	void stepPhysics(std::vector<Ball>& gameBalls, Players& gamePlayers, TurnInformation& turn)
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
							resolveCircleCollisionPosition(ball, checkTarget);
							resolveCircleCollisionVelocity(ball, checkTarget);

							if (turn.firstHitBallType == BallType::unknown)
							{
								// assume that first collision always is cue ball + random ball
								turn.firstHitBallType = (ball.getBallNumber() == 0) ? checkTarget.getBallType() : ball.getBallType();
								turn.didFoulNoRail = true;
							}

							hasCollided = true; // if a collision occured, then stop moving using old velocity
						}
					}

					--stepsNeeded;
				}

				if (ball.isInPocket())
				{
					ball.setVisible(false);
					ball.setVelocity(0, 0);
					turn.pocketedBalls.push_back(&ball);
					if (gamePlayers.getCurrentPlayer().targetBallType == BallType::unknown)
					{
						if (ball.getBallType() == BallType::solid || ball.getBallType() == BallType::striped)
						{
							turn.isTargetBallsSelectedThisTurn = true;
							gamePlayers.getCurrentPlayer().targetBallType = ball.getBallType();
							gamePlayers.getNextPlayer().targetBallType = (ball.getBallType() == BallType::solid) ? BallType::striped : BallType::solid;
						}
					}
					if (turn.didFoulNoRail)
					{
						turn.didFoulNoRail = false;
					}
				}

				ball.applyFriction(consts::rollingFriction, consts::stoppingVelocity);

				bool didCollide{};
				resolveCircleBoundaryCollision(ball, consts::playSurface, didCollide);

				if (turn.didFoulNoRail && didCollide)
				{
					turn.didFoulNoRail = false;
				}
			}

		}
	}

} // namespace physics

