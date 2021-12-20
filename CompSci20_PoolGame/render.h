#pragma once

#include "Ball.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <vector>

namespace render
{
	void renderFrame(const std::vector<Ball>& vecBalls, const ALLEGRO_MOUSE_STATE& mouseState, const bool isMouseDown, ALLEGRO_FONT*& gameFont, bool allBallsStopped);
}