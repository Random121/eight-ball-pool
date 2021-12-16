#pragma once

#include <tuple>
#include <iostream>

class Ball
{
public:
	using position2d_type = std::tuple<double, double>;
	using velocity2d_type = std::tuple<double, double>;

private:
	double m_xPosition{};
	double m_yPosition{};

	double m_xVelocity{};
	double m_yVelocity{};

	double m_radius{};

public:
	Ball() = default;
	Ball(double xPos, double yPos, double radius);

	position2d_type getPosition2d() const;
	velocity2d_type getVelocity2d() const;

	void setVelocity(const double xVel, const double yVel);
	void setPosition(const double xPos, const double yPos);

	void setRadius(const double radius);
	double getRadius() const;

	bool isOverlappingBall(const Ball& otherBall) const;
	void update();

private:
	void movementStep(const double friction, const double stopVelocity);
	void wallCollisionStep(const int startX, const int startY, const int endX, const int endY);

};

