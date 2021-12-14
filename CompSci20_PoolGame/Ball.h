#pragma once

#include <tuple>
#include <iostream>

class Ball
{
private:
	double m_xPosition{};
	double m_yPosition{};
	double m_xVelocity{};
	double m_yVelocity{};

public:
	Ball() = default;
	Ball(double x, double y);

	std::tuple<double, double> getPosition();
	std::tuple<double, double> getVelocity();

	void setVelocity(double xVel, double yVel);
	void setPosition(double xPos, double yPos);

	void update();

private:
	void movementStep();
	void collisionStep();
};

