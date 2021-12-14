#include "Ball.h"

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

void Ball::collision()
{
    if (m_xPosition - 30 < 0)
    {
        m_xPosition += (0 - (m_xPosition - 30));
        m_xVelocity *= -1;
    }
    else if (m_xPosition + 30 > 640)
    {
        m_xPosition -= (m_xPosition + 30 - 640);
        m_xVelocity *= -1;
    }

    // check for boundaries in y-axis
    if (m_yPosition - 30 < 0)
    {
        m_yPosition += (0 - (m_yPosition - 30));
        m_yVelocity *= -1;
    }
    else if (m_yPosition + 30 > 480)
    {
        m_yPosition -= (m_yPosition + 30 - 480);
        m_yVelocity *= -1;
    }
}

void Ball::stepMovement()
{
	m_xPosition += m_xVelocity;
	m_yPosition += m_yVelocity;

    int dirX{ (m_xVelocity > 0) - (m_xVelocity < 0) };
    int dirY{ (m_yVelocity > 0) - (m_yVelocity < 0) };

    m_xVelocity -= 0.01 * dirX;
    m_yVelocity -= 0.01 * dirY;

    std::cout << m_xVelocity << ' ' << m_yVelocity << '\n';

    if (std::abs(m_xVelocity) < 0.01 && std::abs(m_yVelocity) < 0.01)
        setVelocity(0, 0);

    collision();
}
