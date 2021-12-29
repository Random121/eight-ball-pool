#pragma once

#include "Ball.h"
#include "CueStick.h"

#include <allegro5/allegro_font.h>

#include <vector>

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, const ALLEGRO_FONT* const& gameFont);
	void drawPockets();
	void drawCueStick(CueStick stick);
	void drawPlaysurface();
	void renderDrawings();
}