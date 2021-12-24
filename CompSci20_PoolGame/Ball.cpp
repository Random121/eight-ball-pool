#include "Ball.h"

#include "utilities.h"
#include "constants.h"

#include <cmath>
#include <iostream>

Ball::Ball(double xPos, double yPos, double radius, double mass)
	: m_xPosition{ xPos }, m_yPosition{ yPos }, m_radius{ radius }, m_mass{ mass }
{
}

void Ball::setPosition(const double xPos, const double yPos)
{
	m_xPosition = xPos;
	m_yPosition = yPos;
}

void Ball::addPosition(const double xPos, const double yPos)
{
	m_xPosition += xPos;
	m_yPosition += yPos;
}

void Ball::subPosition(const double xPos, const double yPos)
{
	m_xPosition -= xPos;
	m_yPosition -= yPos;
}

double Ball::getX() const
{
	return m_xPosition;
}

double Ball::getY() const
{
	return m_yPosition;
}

void Ball::setVelocity(const double xVel, const double yVel)
{
	m_xVelocity = xVel;
	m_yVelocity = yVel;
}

void Ball::addVelocity(const double xVel, const double yVel)
{
	m_xVelocity += xVel;
	m_yVelocity += yVel;
}

void Ball::subVelocity(const double xVel, const double yVel)
{
	m_xVelocity -= xVel;
	m_yVelocity -= yVel;
}

double Ball::getVX() const
{
	return m_xVelocity;
}

double Ball::getVY() const
{
	return m_yVelocity;
}

void Ball::setRadius(const double radius)
{
	if (radius > 0.0)
		m_radius = radius;

#ifdef DEBUG
	else
		std::cout << "Attempted to set negative radius: " << radius << '\n';
#endif // DEBUG
}

double Ball::getRadius() const
{
	return m_radius;
}

void Ball::setMass(const double mass)
{
	if (mass > 0.0)
		m_mass = mass;

#ifdef DEBUG
	else
		std::cout << "Attempted to set negative mass: " << mass << '\n';
#endif // DEBUG
}

double Ball::getMass() const
{
	return m_mass;
}

bool Ball::isMoving() const
{
	return (m_xVelocity * m_xVelocity + m_yVelocity * m_yVelocity) > 0.0;
}

bool Ball::isVisible() const
{
	return m_isVisible;
}

void Ball::setVisible(bool visibility)
{
	m_isVisible = visibility;
}

int Ball::getBallNumber() const
{
	return m_ballNumber;
}

void Ball::setBallNumber(int number)
{
	m_ballNumber = number;
}

BallType Ball::getBallType() const
{
	if (m_ballNumber == 8)
		return BallType::eight;

	if (m_ballNumber > 8)
		return BallType::striped;
	else
		return BallType::solid;

}

void Ball::applyFriction(const double friction, const double stopVelocity)
{
	// stop ball if the net velocity is near zero
	if ((m_xVelocity * m_xVelocity + m_yVelocity * m_yVelocity) < stopVelocity)
	{
		m_xVelocity = 0.0;
		m_yVelocity = 0.0;
	}
	else
	{
		// apply rolling friction
		m_xVelocity -= m_xVelocity * friction;
		m_yVelocity -= m_yVelocity * friction;
	}
}

// Calculations from: https://github.com/liballeg/allegro_wiki/wiki/Circle-Collision-2D
bool Ball::isOverlappingBall(const Ball& otherBall) const
{
	// compare pointers to ensure that the collision is not with itself
	// and that the other ball is active
	if (&otherBall == this || !otherBall.isVisible())
		return false;

	const double radiusLength{ m_radius + otherBall.getRadius() };
	const double deltaX{ m_xPosition - otherBall.getX() };
	const double deltaY{ m_yPosition - otherBall.getY() };

#ifdef DEBUG
	if (((deltaX * deltaX) + (deltaY * deltaY)) <= (radiusLength * radiusLength))
	{
		std::cout << "[OVERLAPPING INFO]\n";
		std::cout << "[self " << this << "] " << m_xPosition << ", " << m_yPosition << ", " << m_radius << '\n';
		std::cout << "[other " << &otherBall << "] " << otherBall.getX() << ", " << otherBall.getY() << ", " << otherBall.getRadius() << "\n\n";
		return true;
	}
	return false;
#endif // DEBUG

	return (deltaX * deltaX + deltaY * deltaY) <= (radiusLength * radiusLength);
}

bool Ball::isInPocket() const
{
	for (const std::vector<int>& pocketCoord : consts::pocketCoordinates)
	{
		const double radiusLength{ m_radius + consts::pocketRadius - 10 };
		const double deltaX{ m_xPosition - pocketCoord[0] };
		const double deltaY{ m_yPosition - pocketCoord[1] };

		if ((deltaX * deltaX + deltaY * deltaY) <= (radiusLength * radiusLength))
		{
#ifdef DEBUG
			std::cout << "IN POCKET" << '\n';,
#endif // DEBUG
			return true;
		}
	}
	return false;
}