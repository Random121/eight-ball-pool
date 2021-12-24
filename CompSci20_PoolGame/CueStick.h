#pragma once

#include "Input.h"

class CueStick
{
private:
	bool m_canUpdate{};
	bool m_isVisible{};
	int m_cuePower{};

	Input& m_input{ Input::getInstance() };

	void updatePower();
	void updatePosition(const double xPos, const double yPos);

public:
	CueStick() = default;
	CueStick(bool canUpdate, bool visible);

	// way easier just to have it as public

	double m_outlineStartX{};
	double m_outlineStartY{};
	double m_outlineEndX{};
	double m_outlineEndY{};

	double m_stickStartX{};
	double m_stickStartY{};
	double m_stickEndX{};
	double m_stickEndY{};

	bool canUpdate() const;
	void setCanUpdate(bool canUpdate);

	int getCuePower() const;
	void setCuePower(int power);

	bool isVisible() const;
	void setVisible(bool visible);

	void updateAll(const double xPos, const double yPos);
};

