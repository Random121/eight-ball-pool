#pragma once

class Vector2
{
private:
	double m_x{};
	double m_y{};

public:

	Vector2() = default;
	Vector2(const double x, const double y);

	double getX() const;
	double getY() const;
	
	void setX(const double x);
	void setY(const double y);
	void setXY(const double x, const double y);

	void addToX(const double x);	
	void addToY(const double y);

	Vector2 copyAndAdd(const Vector2& vec2) const;
	Vector2 copyAndSubtract(const Vector2& vec2) const;
	Vector2 copyAndMultiply(const double scale) const;

	void add(const Vector2& vec2);
	void subtract(const Vector2& vec2);
	void multiply(const double scale);

	double getLength() const;
	// math behind dot product from here https://www.mathsisfun.com/algebra/vectors-dot-product.html
	double getDotProduct(const Vector2& vec2) const;
	// normalizing the vector means that the length of it is equal to 1
	Vector2 getNormalized() const;
};
