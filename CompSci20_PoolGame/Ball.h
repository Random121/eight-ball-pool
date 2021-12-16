#pragma once

#include <iostream>

class Ball
{
private:
	double m_xPosition{};
	double m_yPosition{};

	double m_xVelocity{};
	double m_yVelocity{};

	double m_radius{};

public:
	Ball() = default;
	Ball(double xPos, double yPos, double radius);

	// could not think of a better way to do this
	double getX() const;
	double getY() const;
	double getVX() const;
	double getVY() const;

	void setPosition(const double xPos, const double yPos);
	void setVelocity(const double xVel, const double yVel);

	void addPosition(const double xPos, const double yPos);
	void addVelocity(const double xVel, const double yVel);

	void setRadius(const double radius);
	double getRadius() const;

	bool isOverlappingBall(const Ball& otherBall) const;
	void update();

private:
	void movementStep(const double friction, const double stopVelocity);
	void wallCollisionStep(const int startX, const int startY, const int endX, const int endY);

};

