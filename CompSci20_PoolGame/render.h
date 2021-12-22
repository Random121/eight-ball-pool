#pragma once

#include "Ball.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <vector>

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, ALLEGRO_FONT*& gameFont);
	void drawPockets();
	void drawCueStick(const Ball& cueBall, const ALLEGRO_MOUSE_STATE& mouseState, const double stickPower);
	void drawPlaysurface();
	//void renderFrame(const std::vector<Ball>& gameBalls, const bool isMouseDown, const ALLEGRO_MOUSE_STATE& mouseState, ALLEGRO_FONT*& gameFont, const bool allBallsStopped, const double stickPower);
}