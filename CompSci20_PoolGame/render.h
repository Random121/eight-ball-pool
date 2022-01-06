#pragma once

#include "Ball.h"
#include "CueStick.h"

#include <allegro5/allegro_font.h>

namespace render
{
	void drawBalls(const Ball::balls_type& gameBalls, ALLEGRO_FONT* const& gameFont);
	void drawPockets();
	void drawCueStick(CueStick stick);
	void drawPlaysurface();
	void renderDrawings();
}
