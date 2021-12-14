#include "Ball.h"
#include "utils.h"

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
	if (m_xPosition - 10 < 0)
	{
		m_xPosition += (0 - (m_xPosition - 10));
		m_xVelocity *= -1;
	}
	else if (m_xPosition + 10 > 640)
	{
		m_xPosition -= (m_xPosition + 10 - 640);
		m_xVelocity *= -1;
	}

	// check for boundaries in y-axis
	if (m_yPosition - 10 < 0)
	{
		m_yPosition += (0 - (m_yPosition - 10));
		m_yVelocity *= -1;
	}
	else if (m_yPosition + 10 > 480)
	{
		m_yPosition -= (m_yPosition + 10 - 480);
		m_yVelocity *= -1;
	}
}

// TODO: header file for all constants
#define FRICTION_VAL 0.01
#define STOP_VELOCITY 0.5

void Ball::movementStep()
{
	// actually move ball forward
	m_xPosition += m_xVelocity;
	m_yPosition += m_yVelocity;

	// stop ball completely if it can't be slowed down further
	if (std::abs(m_xVelocity) < STOP_VELOCITY && std::abs(m_yVelocity) < STOP_VELOCITY)
	{
		setVelocity(0, 0);
	}
	else //simulate friction slowing
	{
		//m_xVelocity -= FRICTION_VAL * getSign(m_xVelocity);
		//m_yVelocity -= FRICTION_VAL * getSign(m_yVelocity);

		m_xVelocity -= m_xVelocity * FRICTION_VAL;
		m_yVelocity -= m_yVelocity * FRICTION_VAL;
	}

	std::cout << m_xVelocity << ' ' << m_yVelocity << '\n';
}

// wrapper to call stuff
void Ball::update()
{
	movementStep();
	collisionStep();
}