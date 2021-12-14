#include "Ball.h"
#include "utils.h"
#include "constants.h"

#include <tuple>
#include <cmath>
#include <iostream>

Ball::Ball(double xPos, double yPos) : m_xPosition{ xPos }, m_yPosition{ yPos }
{
}

std::tuple<double, double> Ball::getPosition()
{
	return std::make_tuple(m_xPosition, m_yPosition);
}

std::tuple<double, double> Ball::getVelocity()
{
	return std::make_tuple(m_xVelocity, m_yVelocity);
}

void Ball::setVelocity(double xVel, double yVel)
{
	m_xVelocity = xVel;
	m_yVelocity = yVel;
}

void Ball::setPosition(double xPos, double yPos)
{
	m_xPosition = xPos;
	m_yPosition = yPos;
}

void Ball::collisionStep()
{
	if (m_xPosition - consts::ballRadius < consts::playSurfaceX)
	{
		m_xPosition += (consts::playSurfaceX - (m_xPosition - consts::ballRadius));
		m_xVelocity *= -1;
	}
	else if (m_xPosition + consts::ballRadius > consts::screenWidth - consts::playSurfaceX)
	{
		m_xPosition -= (m_xPosition + consts::ballRadius - BOUNDING_BOX_BOTTOM_X);
		m_xVelocity *= -1;
	}

	// check for boundaries in y-axis
	if (m_yPosition - consts::ballRadius < consts::playSurfaceY)
	{
		m_yPosition += (consts::playSurfaceY - (m_yPosition - consts::ballRadius));
		m_yVelocity *= -1;
	}
	else if (m_yPosition + consts::ballRadius > consts::screenHeight - consts::playSurfaceY)
	{
		m_yPosition -= (m_yPosition + consts::ballRadius - BOUNDING_BOX_BOTTOM_Y);
		m_yVelocity *= -1;
	}
}

void Ball::movementStep()
{
	// actually move ball forward
	m_xPosition += m_xVelocity;
	m_yPosition += m_yVelocity;

	// stop ball completely if it can't be slowed down further
	if (std::abs(m_xVelocity) < consts::stoppingVelocity && std::abs(m_yVelocity) < consts::stoppingVelocity)
	{
		setVelocity(0, 0);
	}
	else //simulate friction slowing
	{
		//m_xVelocity -= FRICTION_VAL * getSign(m_xVelocity);
		//m_yVelocity -= FRICTION_VAL * getSign(m_yVelocity);

		m_xVelocity -= m_xVelocity * consts::frictionValue;
		m_yVelocity -= m_yVelocity * consts::frictionValue;
	}

	std::cout << m_xVelocity << ' ' << m_yVelocity << '\n';
}

// wrapper to call stuff
void Ball::update()
{
	movementStep();
	collisionStep();
}
