#include "Ball.h"

#include "common.h"
#include "constants.h"
#include "Vector2.h"

#include <iostream>
#include <cmath>
#include <array>

// constructors

Ball::Ball(const double xPos, const double yPos, const double radius, const double mass)
	: m_position{ xPos, yPos }, m_radius{ radius }, m_mass{ mass }
{
}

Ball::Ball(const Vector2& posVector, const double radius, const double mass)
	: m_position{ posVector }, m_radius{ radius }, m_mass{ mass }
{
}

// position getters

double Ball::getX() const
{
	return m_position.getX();
}

double Ball::getY() const
{
	return m_position.getY();
}

// velocity getters

double Ball::getVX() const
{
	return m_velocity.getX();
}

double Ball::getVY() const
{
	return m_velocity.getY();
}

// vector getters

Vector2 Ball::getPositionVector() const
{
	return m_position;
}

Vector2 Ball::getVelocityVector() const
{
	return m_velocity;
}

// position setters

void Ball::setPosition(const double xPos, const double yPos)
{
	m_position.setXY(xPos, yPos);
}

void Ball::addPosition(const double xPos, const double yPos)
{
	m_position.addToX(xPos);
	m_position.addToY(yPos);
}

void Ball::subPosition(const double xPos, const double yPos)
{
	m_position.addToX(-xPos);
	m_position.addToY(-yPos);
}

void Ball::setPosition(const Vector2& posVector)
{
	m_position.setXY(posVector.getX(), posVector.getY());
}

void Ball::addPosition(const Vector2& posVector)
{
	m_position.add(posVector);
}

void Ball::subPosition(const Vector2& posVector)
{
	m_position.subtract(posVector);
}

// velocity setters

void Ball::setVelocity(const double xVel, const double yVel)
{
	m_velocity.setXY(xVel, yVel);
}

void Ball::addVelocity(const double xVel, const double yVel)
{
	m_velocity.addToX(xVel);
	m_velocity.addToY(yVel);
}

void Ball::subVelocity(const double xVel, const double yVel)
{
	m_velocity.addToX(-xVel);
	m_velocity.addToY(-yVel);
}

void Ball::setVelocity(const Vector2& velVector)
{
	m_velocity.setXY(velVector.getX(), velVector.getY());
}

void Ball::addVelocity(const Vector2& velVector)
{
	m_velocity.add(velVector);
}

void Ball::subVelocity(const Vector2& velVector)
{
	m_velocity.subtract(velVector);
}

// radius getter and setter

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

// mass getter and setter

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

// visible getter and setter

void Ball::setVisible(bool visibility)
{
	m_isVisible = visibility;
}

bool Ball::isVisible() const
{
	return m_isVisible;
}

// ball number getter and setter

void Ball::setBallNumber(int number)
{
	m_ballNumber = number;
}

int Ball::getBallNumber() const
{
	return m_ballNumber;
}

BallType Ball::getBallType() const
{
	if (m_ballNumber == 0)
		return BallType::cue;

	if (m_ballNumber == 8)
		return BallType::eight;

	if (m_ballNumber > 8)
		return BallType::striped;
	else
		return BallType::solid;
}

bool Ball::isMoving() const
{
	// dot product is way more efficient than calculating the square root
	return m_velocity.getDotProduct(m_velocity) > 0;
}

void Ball::applyFriction(const double friction, const double stopVelocity)
{
	// stop ball if the net velocity is near zero
	if (m_velocity.getDotProduct(m_velocity) < stopVelocity)
	{
		m_velocity.setXY(0, 0);
	}
	else // apply rolling friction
	{
		m_velocity.subtract(m_velocity.copyAndMultiply(friction));
	}
}

bool Ball::isOverlappingBall(const Ball& otherBall) const
{
	// ensure that the collision is not with itself and that the other ball is active
	if (&otherBall == this || !otherBall.isVisible())
		return false;

	const double radiusLength{ m_radius + otherBall.getRadius() };
	const Vector2 deltaPosition{ m_position.copyAndSubtract(otherBall.m_position) };

#ifdef DEBUG
	if (deltaPosition.getDotProduct(deltaPosition) <= (radiusLength * radiusLength))
	{
		std::cout << "[OVERLAPPING INFO]\n";
		std::cout << "[self " << this << "] " << getX() << ", " << getY() << ", " << m_radius << '\n';
		std::cout << "[other " << &otherBall << "] " << otherBall.getX() << ", " << otherBall.getY() << ", " << otherBall.getRadius() << "\n\n";
		return true;
	}
	return false;
#endif // DEBUG

	return deltaPosition.getDotProduct(deltaPosition) <= (radiusLength * radiusLength);
}

bool Ball::isInPocket() const
{
	for (const std::array<int, 2>&pocketCoord : consts::pocketCoordinates)
	{
		const double radiusLength{ m_radius + consts::pocketRadius - 10 };
		const double deltaX{ m_position.getX() - pocketCoord[0] };
		const double deltaY{ m_position.getY() - pocketCoord[1] };

		if ((deltaX * deltaX + deltaY * deltaY) <= (radiusLength * radiusLength))
		{
#ifdef DEBUG
			std::cout << "IN POCKET" << '\n';
#endif // DEBUG
			return true;
		}
	}
	return false;
}
