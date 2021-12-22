#pragma once

class Ball
{
private:
	double m_xPosition{};
	double m_yPosition{};

	double m_xVelocity{};
	double m_yVelocity{};

	double m_radius{};
	double m_mass{};

	bool m_isVisible{};

public:
	Ball() = default;
	Ball(double xPos, double yPos, double radius, double mass);

	double getX() const;
	double getY() const;
	double getVX() const;
	double getVY() const;

	// did not need the subtract functions, but it is way
	// nicer to interpret than adding negative numbers

	void setPosition(const double xPos, const double yPos);
	void setVelocity(const double xVel, const double yVel);
	void subPosition(const double xPos, const double yPos);

	void addPosition(const double xPos, const double yPos);
	void addVelocity(const double xVel, const double yVel);
	void subVelocity(const double xVel, const double yVel);

	void setRadius(const double radius);
	double getRadius() const;

	void setMass(const double mass);
	double getMass() const;

	bool isMoving() const;

	bool isVisible() const;
	void setVisible(bool visibility);

	void applyFriction(const double friction, const double stopVelocity);

	bool isOverlappingBall(const Ball& otherBall) const;
	bool isInPocket() const;
};

