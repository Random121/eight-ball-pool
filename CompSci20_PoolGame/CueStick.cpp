#include "CueStick.h"

#include "constants.h"

#include <allegro5/allegro5.h>

CueStick::CueStick(bool canUpdate, bool visible)
	: m_canUpdate{ canUpdate }, m_isVisible{ visible }
{
}

void CueStick::updatePower()
{
	if (m_input.isKeyDown(ALLEGRO_KEY_W) && m_cuePower < 70)
	{
		m_cuePower += 1;
	}
	else if (m_input.isKeyDown(ALLEGRO_KEY_S) && m_cuePower > 0)
	{
		m_cuePower -= 1;
	}
}

void CueStick::updatePosition(const double xPos, const double yPos)
{
	const double cueDistanceFromPosition{ consts::cueStickDistanceFromBall + m_cuePower };
	const double cueStickLength{ consts::cueStickLength + m_cuePower };

	const double deltaX{ xPos - m_input.getMouseX() };
	const double deltaY{ yPos - m_input.getMouseY() };
	const double mouseBallDistance{ calculateHypotenuse(deltaX, deltaY) };

	const double vectorShrinkFactor{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / (cueDistanceFromPosition * cueDistanceFromPosition)) };
	const double vectorShrinkFactorWood{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / ((cueDistanceFromPosition + 4) * (cueDistanceFromPosition + 4))) };

	m_outlineStartX = xPos + (deltaX / vectorShrinkFactor);
	m_outlineStartY = yPos + (deltaY / vectorShrinkFactor);
	m_outlineEndX = xPos + (deltaX / mouseBallDistance) * cueStickLength;
	m_outlineEndY = yPos + (deltaY / mouseBallDistance) * cueStickLength;

	m_stickStartX = xPos + (deltaX / vectorShrinkFactorWood);
	m_stickStartY = yPos + (deltaY / vectorShrinkFactorWood);
	m_stickEndX = xPos + (deltaX / mouseBallDistance) * (cueStickLength - 2);
	m_stickEndY = yPos + (deltaY / mouseBallDistance) * (cueStickLength - 2);
}

void CueStick::updateAll(const double xPos, const double yPos)
{
	if (m_canUpdate)
	{
		updatePower();
		updatePosition(xPos, yPos);
	}
}

bool CueStick::canUpdate() const
{
	return m_canUpdate;
}

void CueStick::setCanUpdate(bool canUpdate)
{
	m_canUpdate = canUpdate;
}

bool CueStick::isVisible() const
{
	return m_isVisible;
}

void CueStick::setVisible(bool visible)
{
	m_isVisible = visible;
}

int CueStick::getCuePower() const
{
	return m_cuePower;
}

void CueStick::setCuePower(int power)
{
	m_cuePower = power;
}
