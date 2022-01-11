#pragma once

#include "Vector2.h"

#include <vector>

class Ball
{
public:
	using balls_type = std::vector<Ball>;
	using ballsPointer_type = std::vector<Ball*>;

	enum class BallSuitType
	{
		unknown,
		solid,
		striped,
		eight,
		cue
	};

private:
	Vector2 m_position{};
	Vector2 m_velocity{};

	double m_radius{};
	double m_mass{};

	bool m_isVisible{};
	int m_ballNumber{};

public:
	Ball() = default;
	Ball(const double xPos, const double yPos, const double radius, const double mass);
	Ball(const Vector2& posVector, const double radius, const double mass);

	double getX() const;
	double getY() const;
	double getVX() const;
	double getVY() const;

	Vector2 getPositionVector() const;
	Vector2 getVelocityVector() const;

	// did not need the subtract functions, but it is way
	// nicer to interpret than adding negative numbers

	void setPosition(const double xPos, const double yPos);
	void addPosition(const double xPos, const double yPos);
	void subPosition(const double xPos, const double yPos);
	void setPosition(const Vector2& posVector);
	void addPosition(const Vector2& posVector);
	void subPosition(const Vector2& posVector);

	void setVelocity(const double xVel, const double yVel);
	void addVelocity(const double xVel, const double yVel);
	void subVelocity(const double xVel, const double yVel);
	void setVelocity(const Vector2& velVector);
	void addVelocity(const Vector2& velVector);
	void subVelocity(const Vector2& velVector);

	void setRadius(const double radius);
	double getRadius() const;

	void setMass(const double mass);
	double getMass() const;

	void setVisible(bool visibility);
	bool isVisible() const;

	void setBallNumber(int number);
	int getBallNumber() const;

	// check if the ball is a normal suit ball (solid or striped)
	bool isSuitBall() const;
	BallSuitType getBallType() const;

	bool isMoving() const;
	void applyFriction(const double friction, const double stopVelocity);

	bool isOverlappingBall(const Ball& otherBall) const;
	bool isInPocket() const;
};
