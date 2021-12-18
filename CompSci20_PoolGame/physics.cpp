#include "physics.h"
#include "Ball.h"
#include "utilities.h"
#include "constants.h"

#include <iostream>
#include <vector>
#include <cstddef>

namespace physics
{
	void resolveCircleCollisionPosition(Ball& ball1, Ball& ball2)
	{
		const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };

		// calculate distance balls should move by to stop overlapping
		const double ballOverlap{ ((ballDistance - ball1.getRadius() - ball2.getRadius()) / 2.0) };

		// change distance into position vectors (x and y)
		const double moveDistanceX{ ballOverlap * (ball1.getX() - ball2.getX()) / ballDistance };
		const double moveDistanceY{ ballOverlap * (ball1.getY() - ball2.getY()) / ballDistance };

#ifdef DEBUG
		std::cout << "[POSITION RESOLUTION]\n";
		std::cout << "[distance] " << ballDistance << '\n';
		std::cout << "[overlap] " << ballOverlap << '\n';
		std::cout << "[moveDistanceX] " << moveDistanceX << '\n';
		std::cout << "[moveDistanceY] " << moveDistanceY << '\n';
		std::cout << "[start ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
		std::cout << "[start ball2] " << ball2.getX() << ", " << ball2.getY() << '\n';
#endif // DEBUG

		// move each ball in opposite directions to resolve overlap
		ball1.subPosition(moveDistanceX, moveDistanceY);
		ball2.addPosition(moveDistanceX, moveDistanceY);

#ifdef DEBUG
		std::cout << "[end ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
		std::cout << "[end ball2] " << ball2.getX() << ", " << ball2.getY() << "\n\n";
#endif // DEBUG
	}

	void resolveCircleCollisionVelocity(Ball& ball1, Ball& ball2)
	{
		const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };

		// normal vector (from center to center)
		const double normalX{ (ball2.getX() - ball1.getX()) / ballDistance };
		const double normalY{ (ball2.getY() - ball1.getY()) / ballDistance };

		const double deltaVelocityX{ ball1.getVX() - ball2.getVX() };
		const double deltaVelocityY{ ball1.getVY() - ball2.getVY() };

		// solve conservation of momentum
		const double p{ 2.0 * dotProduct(normalX, deltaVelocityX, normalY, deltaVelocityY) / (ball1.getMass() + ball2.getMass()) };
		const double newVelocityX{ p * normalX * consts::collisionFriction };
		const double newVelocityY{ p * normalY * consts::collisionFriction };

#ifdef DEBUG
		std::cout << "[VELOCITY RESOLUTION]\n";
		std::cout << "[distance] " << ballDistance << '\n';
		std::cout << "[normal] " << normalX << ", " << normalY << '\n';
		std::cout << "[deltaVelocity] " << deltaVelocityX << ", " << deltaVelocityY << '\n';
		std::cout << "[newVelocity] " << newVelocityX << ", " << newVelocityY << '\n';
		std::cout << "[start ball1] " << ball1.getVX() << ", " << ball1.getVY() << '\n';
		std::cout << "[start ball2] " << ball2.getVX() << ", " << ball2.getVY() << '\n';
#endif // DEBUG

		ball1.subVelocity(
			newVelocityX * ball2.getMass(),
			newVelocityY * ball2.getMass()
		);

		ball2.addVelocity(
			newVelocityX * ball1.getMass(),
			newVelocityY * ball1.getMass()
		);

#ifdef DEBUG
		std::cout << "[end ball1] " << ball1.getVX() << ", " << ball1.getVY() << '\n';
		std::cout << "[end ball2] " << ball2.getVX() << ", " << ball2.getVY() << "\n\n";
#endif // DEBUG
	}

	void resolveCircleBoundaryCollision(Ball& ball, const Rectangle& boundaries)
	{
		double xPositionAdjustment{ 0 };
		double yPositionAdjustment{ 0 };

		// check for boundaries in x-axis
		if (ball.getX() - ball.getRadius() < boundaries.xPos1)
		{
			xPositionAdjustment = (boundaries.xPos1 - (ball.getX() - ball.getRadius()));
			ball.setVelocity(-consts::collisionFriction * ball.getVX(), ball.getVY());
		}
		else if (ball.getX() + ball.getRadius() > boundaries.xPos2)
		{
			xPositionAdjustment = -(ball.getX() + ball.getRadius() - boundaries.xPos2);
			ball.setVelocity(-consts::collisionFriction * ball.getVX(), ball.getVY());
		}

		// check for boundaries in y-axis
		if (ball.getY() - ball.getRadius() < boundaries.yPos1)
		{
			yPositionAdjustment = (boundaries.yPos1 - (ball.getY() - ball.getRadius()));
			ball.setVelocity(ball.getVX(), -consts::collisionFriction * ball.getVY());
		}
		else if (ball.getY() + ball.getRadius() > boundaries.yPos2)
		{
			yPositionAdjustment = -(ball.getY() + ball.getRadius() - boundaries.yPos2);
			ball.setVelocity(ball.getVX(), -consts::collisionFriction * ball.getVY());
		}

		if (xPositionAdjustment != 0 || yPositionAdjustment != 0)
			ball.addPosition(xPositionAdjustment, yPositionAdjustment);
	}

	void advanceBallPosition(Ball& ball, const double friction, const double stopVelocity)
	{
		// stop ball if the net velocity is near zero
		if ((ball.getVX() * ball.getVX() + ball.getVY() * ball.getVY()) < stopVelocity)
		{
			ball.setVelocity(0.0, 0.0);
			return;
		}

		//ball.subVelocity(ball.getVX() * friction, ball.getVY() * friction);
		ball.setVelocity(ball.getVX() * friction, ball.getVY() * friction);

		ball.addPosition(ball.getVX(), ball.getVY());

#ifdef DEBUG
		std::cout << "[Ball " << &ball << "]: " << ball.getX() << ", " << ball.getY() << '\n';
#endif // DEBUG
	}

	/*
		--IMPORTANT REMINDER FOR COLLISION TESTING--
		We MUST check every ball against every other ball for collision.
		It would be great to not have duplicate checks but an unchecked
		ball could move into another ball. This unchecked collision will
		only be detected once the checked ball has moved from its
		stationary location and will lead to incorrect position and
		velocity resolutions where they either go through each other or =
		continuously swap positions in place.

		Conclusion: don't replace the range based for loops below with the
		commented out ones.
	*/

	void stepPhysics(std::vector<Ball>& vecBalls)
	{
		const std::size_t vecBallsSize{ vecBalls.size() };

		//for (int i{}; i < vecBallsSize; ++i)
		for (Ball& ball : vecBalls)
		{
			//Ball& ball{ vecBalls[i] };
			const double velocitySum{ std::abs(ball.getVX()) + std::abs(ball.getVY()) };
			double stepsNeeded{ std::ceil(velocitySum / ball.getRadius()) };
			if (stepsNeeded > 0.0)
			{
				const double stepSizeX{ ball.getVX() / stepsNeeded };
				const double stepSizeY{ ball.getVY() / stepsNeeded };

#ifdef DEBUG
				std::cout << "[STEP MOVE " << &ball << "] "
					<< stepSizeX << ", " << stepSizeY << ", " << stepsNeeded << ", "
					<< ball.getX() << ", " << ball.getY() << '\n';
#endif // DEBUG

				while (stepsNeeded > 0.0)
				{
					ball.addPosition(stepSizeX, stepSizeY);

					//for (int j{ i + 1 }; j < vecBallsSize; ++j)
					for (Ball& checkTargetBall : vecBalls)
					{
						//Ball& checkTargetBall{ vecBalls[j] };
						if (ball.isOverlappingBall(checkTargetBall))
						{
							resolveCircleCollisionPosition(ball, checkTargetBall);
							resolveCircleCollisionVelocity(ball, checkTargetBall);
							stepsNeeded = 0; // stop moving forward as velocity has now changed
						}
					}

					--stepsNeeded;
				}

				resolveCircleBoundaryCollision(ball, consts::playSurface);

				// stop ball if the net velocity is near zero
				if ((ball.getVX() * ball.getVX() + ball.getVY() * ball.getVY()) < consts::stoppingVelocity)
				{
					ball.setVelocity(0.0, 0.0);
				}
				else // apply rolling friction
				{
					ball.setVelocity(ball.getVX() * consts::rollingFriction, ball.getVY() * consts::rollingFriction);
				}
			}
			else // collision checks for stationary balls (not essential, just as a precaution)
			{
				for (Ball& ball2 : vecBalls)
				{
					if (ball.isOverlappingBall(ball2))
					{
						resolveCircleCollisionPosition(ball, ball2);
						resolveCircleCollisionVelocity(ball, ball2);
					}

					resolveCircleBoundaryCollision(ball, consts::playSurface);
				}
			}
		}

		/*
			Old code for collision checks that did not have multisampling
			checks. It often missed collisions for fast oving balls.
		*/

		/*
		for (Ball& ball : vecBalls)
		{
			advanceBallPosition(ball, consts::rollingFriction, consts::stoppingVelocity);
			resolveCircleBoundaryCollision(ball, consts::playSurface);
		}

		// vector of objects which have collided and will have velocities resolved later
		std::vector<std::pair<Ball*, Ball*>> collidedBallPairs;

		for (int i{}; i < vecBalls.size(); ++i)
		{
			for (int j{ i + 1 }; j < vecBalls.size(); ++j)
			{
				Ball& ball1{ vecBalls[i] };
				Ball& ball2{ vecBalls[j] };
				if (ball1.isOverlappingBall(ball2))
				{
					collidedBallPairs.push_back(std::make_pair(&ball1, &ball2));
					resolveCircleCollisionPosition(ball1, ball2);
				}
			}
		}

		for (std::pair<Ball*, Ball*>& ballPair : collidedBallPairs)
		{
			Ball& ball1{ *(ballPair.first) };
			Ball& ball2{ *(ballPair.second) };
			resolveCircleCollisionVelocity(ball1, ball2);
		}
		*/
	}

} // namespace physics

