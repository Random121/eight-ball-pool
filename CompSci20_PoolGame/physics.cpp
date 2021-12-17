#include "physics.h"
#include "Ball.h"
#include "utilities.h"
#include "constants.h"

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
	ball1.addPosition(-moveDistanceX, -moveDistanceY);
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
	const double normalX{ (ball1.getX() - ball2.getX()) / ballDistance };
	const double normalY{ (ball1.getY() - ball2.getY()) / ballDistance };

	const double deltaVelocityX{ ball1.getVX() - ball2.getVX() };
	const double deltaVelocityY{ ball1.getVY() - ball2.getVY() };

	// solve conservation of momentum
	const double p{ 2.0 * dotProduct(normalX, deltaVelocityX, normalY, deltaVelocityY) / (ball1.getMass() + ball2.getMass()) };
	const double newVelocityX{ p * normalX };
	const double newVelocityY{ p * normalY };

#ifdef DEBUG
	std::cout << "[START VELOCITY] " << ball1.getVX() << ", " << ball2.getVX() << ", " << ballDistance << '\n';
#endif // DEBUG


	ball1.addVelocity(
		-(newVelocityX * ball2.getMass()),
		-(newVelocityY * ball2.getMass())
	);

	ball2.addVelocity(
		(newVelocityX * ball1.getMass()),
		(newVelocityY * ball1.getMass())
	);

#ifdef DEBUG
	std::cout << "[END VELOCITY] " << ball1.getVX() << ", " << ball2.getVX() << ", " << ballDistance << "\n\n";
#endif // DEBUG
}