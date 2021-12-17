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
}

double Ball::getRadius() const
{
	return m_radius;
}

void Ball::setMass(const double mass)
{
	if (mass > 0.0)
		m_mass = mass;
}

double Ball::getMass() const
{
	return m_mass;
}

void Ball::wallCollisionStep(const int startX, const int startY, const int endX, const int endY)
{
	// check for boundaries in x-axis
	if ((m_xPosition - m_radius) < startX)
	{
		m_xPosition += (startX - (m_xPosition - m_radius));
		m_xVelocity *= -1;
	}
	else if ((m_xPosition + m_radius) > endX)
	{
		m_xPosition -= (m_xPosition + m_radius - endX);
		m_xVelocity *= -1;
	}

	// check for boundaries in y-axis
	if ((m_yPosition - m_radius) < startY)
	{
		m_yPosition += (startY - (m_yPosition - m_radius));
		m_yVelocity *= -1;
	}
	else if ((m_yPosition + m_radius) > endY)
	{
		m_yPosition -= (m_yPosition + m_radius - endY);
		m_yVelocity *= -1;
	}
}

void Ball::movementStep(const double friction, const double stopVelocity)
{
	// stop ball completely if the net velocity is near zero
	if (calcPythagoreanHyp(m_xVelocity, m_yVelocity) < stopVelocity)
	{
		setVelocity(0, 0);
		return;
	}
	else // simulate friction slowing
	{
		//m_xVelocity -= FRICTION_VAL * getSign(m_xVelocity);
		//m_yVelocity -= FRICTION_VAL * getSign(m_yVelocity);

		m_xVelocity -= m_xVelocity * friction;
		m_yVelocity -= m_yVelocity * friction;
	}

	// actually move ball forward
	m_xPosition += m_xVelocity;
	m_yPosition += m_yVelocity;

#ifdef DEBUG
	std::cout << "[Ball " << this << "]: " << m_xPosition << ' ' << m_yPosition << '\n';
#endif // DEBUG
}

// Calculations from: https://github.com/liballeg/allegro_wiki/wiki/Circle-Collision-2D
bool Ball::isOverlappingBall(const Ball& otherBall) const
{
	// we just compare the pointers to
	// prevent it from colliding with itself
	if (&otherBall == this)
		return false;

	const double radiusLength{ m_radius + otherBall.getRadius() };
	const double deltaX{ m_xPosition - otherBall.getX() };
	const double deltaY{ m_yPosition - otherBall.getY() };

#ifdef DEBUG
	if (((deltaX * deltaX) + (deltaY * deltaY)) <= (radiusLength * radiusLength))
	{
		std::cout << "[OVERLAPPING INFO]\n";
		std::cout << "[self] " << m_xPosition << ", " << m_yPosition << ", " << m_radius << '\n';
		std::cout << "[other] " << otherBall.getX() << ", " << otherBall.getY() << ", " << otherBall.getRadius() << "\n\n";
		return true;
	}
	return false;
#endif // DEBUG

	return ((deltaX * deltaX) + (deltaY * deltaY)) <= (radiusLength * radiusLength);
}

// wrapper to call stuff
void Ball::update()
{
	movementStep(consts::frictionCoefficient, consts::stoppingVelocity);
	wallCollisionStep(consts::playSurfaceX, consts::playSurfaceY, consts::screenWidth - consts::playSurfaceX, consts::screenHeight - consts::playSurfaceY);
}