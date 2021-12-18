#include "physics.h"
#include "Ball.h"
#include "utilities.h"
#include "constants.h"

#include <iostream>
#include <vector>

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
			(newVelocityX * ball2.getMass()),
			(newVelocityY * ball2.getMass())
		);

		ball2.addVelocity(
			(newVelocityX * ball1.getMass()),
			(newVelocityY * ball1.getMass())
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
	
	void advanceBallPosition(Ball& ball, const double friction, const double stopVelocity, const double deltaTime)
	{
		// stop ball completely if the net velocity is near zero
		if (calcPythagoreanHyp(ball.getVX(), ball.getVY()) < stopVelocity)
		{
			ball.setVelocity(0, 0);
			return;
		}
		else // simulate friction slowing
		{
			//ball.subVelocity(
			//	ball.getVX() * friction * deltaTime,
			//	ball.getVY() * friction * deltaTime
			//);

			ball.setVelocity(ball.getVX() * std::pow(friction, deltaTime), ball.getVY() * std::pow(friction, deltaTime));
		}

		ball.addPosition(ball.getVX() * deltaTime, ball.getVY() * deltaTime);

#ifdef DEBUG
		std::cout << "[Ball " << &ball << "]: " << ball.getX() << ", " << ball.getY() << ", " << deltaTime << '\n';
#endif // DEBUG
	}

	void stepPhysics(std::vector<Ball>& vecBalls, const double deltaTime)
	{
		static constexpr Rectangle bound{
			consts::playSurfaceX,
			consts::playSurfaceY,
			consts::screenWidth - consts::playSurfaceX,
			consts::screenHeight - consts::playSurfaceY
		};

		for (Ball& ball : vecBalls)
		{
			advanceBallPosition(ball, consts::rollingFriction, consts::stoppingVelocity, deltaTime);
			resolveCircleBoundaryCollision(ball, bound);
		}

		// vector of objects which have collided and will have velocities resolved later
		std::vector<std::pair<Ball*, Ball*>> collidedBallPairs;

		for (int i{ 0 }; i < vecBalls.size(); ++i)
		{
			for (int j{ i + 1 }; j < vecBalls.size(); ++j)
			{
				Ball& ball1{ vecBalls[i] };
				Ball& ball2{ vecBalls[j] };
				if (ball1.isOverlappingBall(ball2))
				{
					collidedBallPairs.push_back(std::make_pair(&ball1, &ball2));
					resolveCircleCollisionPosition(ball1, ball2);
					resolveCircleCollisionVelocity(ball1, ball2);
				}
			}
		}

		//for (std::pair<Ball*, Ball*>& ballPair : collidedBallPairs)
		//{
		//	Ball& ball1{ *(ballPair.first) };
		//	Ball& ball2{ *(ballPair.second) };
		//	resolveCircleCollisionVelocity(ball1, ball2);
		//}
	}

} // namespace physics

