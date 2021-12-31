#include "Vector2.h"

#include <cmath>

Vector2::Vector2(const double x, const double y) : m_x{ x }, m_y{ y }
{
}

double Vector2::getX() const
{
	return m_x;
}

double Vector2::getY() const
{
	return m_y;
}

void Vector2::setX(const double x)
{
	m_x = x;
}

void Vector2::setY(const double y)
{
	m_y = y;
}

void Vector2::setXY(const double x, const double y)
{
	m_x = x;
	m_y = y;
}

void Vector2::addToX(const double x)
{
	m_x += x;
}

void Vector2::addToY(const double y)
{
	m_y += y;
}

Vector2 Vector2::copyAndAdd(const Vector2& vec2) const
{
	return Vector2(m_x + vec2.m_x, m_y + vec2.m_y);
}

Vector2 Vector2::copyAndSubtract(const Vector2& vec2) const
{
	return Vector2(m_x - vec2.m_x, m_y - vec2.m_y);
}

Vector2 Vector2::copyAndMultiply(const double scale) const
{
	return Vector2(m_x * scale, m_y * scale);
}

void Vector2::add(const Vector2& vec2)
{
	m_x += vec2.m_x;
	m_y += vec2.m_y;
}

void Vector2::subtract(const Vector2& vec2)
{
	m_x -= vec2.m_x;
	m_y -= vec2.m_y;
}

void Vector2::multiply(const double scale)
{
	m_x *= scale;
	m_y *= scale;
}

double Vector2::getLength() const
{
	return std::sqrt(m_x * m_x + m_y * m_y);
}

double Vector2::getDotProduct(const Vector2& vec2) const
{
	return m_x * vec2.m_x + m_y * vec2.m_y;
}

Vector2 Vector2::getNormalized() const
{
	Vector2 result{ m_x, m_y };
	const double length{ result.getLength() };
	if (length != 0)
	{
		result.m_x = result.m_x / length;
		result.m_y = result.m_y / length;
	}
	return result;
}
