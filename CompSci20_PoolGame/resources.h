#pragma once

#include "Ball.h"

#include <allegro5/allegro_font.h>

#include <vector>

namespace globals
{
	extern ALLEGRO_FONT* gameFont;
	extern std::vector<Ball> gameBalls;
}