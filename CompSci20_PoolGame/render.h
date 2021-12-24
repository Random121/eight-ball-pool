#pragma once

#include "Ball.h"
#include "CueStick.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <vector>

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, ALLEGRO_FONT*& gameFont);
	void drawPockets();
	//void drawCueStick(const double xPos, const double yPos, const ALLEGRO_MOUSE_STATE& mouseState, const double stickPower);
	void drawCueStick(CueStick stick);
	void drawPlaysurface();
}