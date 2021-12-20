#include "render.h"
#include "Ball.h"
#include "constants.h"
#include "utilities.h"

#include <allegro5/mouse.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include <string>
#include <vector>
#include <cmath>

namespace render
{
	void drawBalls(const std::vector<Ball>& vecBalls, ALLEGRO_FONT*& font)
	{
		int ballCount{};
		for (const Ball& ball : vecBalls)
		{
			al_draw_filled_circle(
				ball.getX(),
				ball.getY(),
				ball.getRadius(),
				al_map_rgb(255, (ballCount == 0) * 255, 255)
			);

			//draw ball border
			al_draw_circle(
				ball.getX(),
				ball.getY(),
				ball.getRadius(),
				al_map_rgb(0, 0, 0),
				consts::ballBorderThickness
			);

			al_draw_text(
				font,
				al_map_rgb(0, 0, 0),
				ball.getX() - 5,
				ball.getY() - 5,
				ALLEGRO_ALIGN_LEFT,
				std::to_string(ballCount).c_str()
			);

			++ballCount;
		}
	}

	void drawPockets()
	{
		static const std::vector<std::vector<int>> coords{
			{45, 45},
			{500, 30},
			{955, 45},
			{45, 455},
			{500, 470},
			{955, 465}
		};

		for (const std::vector<int>& coord : coords)
		{
			al_draw_filled_circle(coord[0], coord[1], 25, al_map_rgb(0, 0, 0));
		}
	}

	void drawCueStick(const Ball& cueBall, const ALLEGRO_MOUSE_STATE& mouseState)
	{
		static constexpr double cueDistanceFromBall{ 15 };
		static constexpr int cueStickLength{ 400 };
		static constexpr int cueStickThickness{ 5 };

		const double deltaX{ cueBall.getX() - mouseState.x };
		const double deltaY{ cueBall.getY() - mouseState.y };
		const double cursorBallDistance{ calculateHypotenuse(deltaX, deltaY) };

		const double vectorShrinkFactor{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / (cueDistanceFromBall * cueDistanceFromBall)) };

		al_draw_line(
			cueBall.getX() + (deltaX / vectorShrinkFactor), // cue start x
			cueBall.getY() + (deltaY / vectorShrinkFactor), // cue start y
			cueBall.getX() + (deltaX / cursorBallDistance) * cueStickLength, // cue end x
			cueBall.getY() + (deltaY / cursorBallDistance) * cueStickLength, // cue end y
			al_map_rgb(0, 0, 0),
			cueStickThickness
		);
	}
		

	void renderFrame(const std::vector<Ball>& vecBalls, const ALLEGRO_MOUSE_STATE& mouseState, const bool isMouseDown, ALLEGRO_FONT*& gameFont, bool allBallsStopped)
	{
		al_clear_to_color(al_map_rgb(181, 101, 29));

		// draw play surface
		al_draw_filled_rectangle(
			consts::playSurface.xPos1,
			consts::playSurface.yPos1,
			consts::playSurface.xPos2,
			consts::playSurface.yPos2,
			al_map_rgb(0, 110, 0)
		);

		drawPockets();
		drawBalls(vecBalls, gameFont);

		if (allBallsStopped)
		{
			drawCueStick(vecBalls[0], mouseState);
			if (isMouseDown)
				al_draw_line(mouseState.x, mouseState.y, vecBalls[0].getX(), vecBalls[0].getY(), al_map_rgb(0, 0, 255), 3);
		}


		al_flip_display();
	}
}