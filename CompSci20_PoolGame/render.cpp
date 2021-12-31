#include "render.h"
#include "Ball.h"
#include "constants.h"
#include "common.h"
#include "CueStick.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <string>
#include <vector>
#include <cmath>
#include <cstddef>
#include <iostream>

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, const ALLEGRO_FONT* const& gameFont)
	{
		for (const Ball& ball : gameBalls)
		{
			// skip rendering the cue ball and non visible balls
			if (ball.getBallNumber() != 0 && ball.isVisible())
			{
				const int ballNumber{ ball.getBallNumber() };
				const BallType type{ ball.getBallType() };

				// draw striped balls
				if (type == BallType::striped)
				{
					// draw the white circle background
					al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(255, 255, 255));

					// get the color map
					const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 9] };

					al_draw_filled_circle(ball.getX(), ball.getY(), 11, al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
				}
				else // draw solid balls including eight ball
				{
					// get the color map
					const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 1] };
					al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
				}

				// draw ball border
				al_draw_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);

				const std::string ballNumberString{ std::to_string(ballNumber) };

				// draw single digit numbers
				if (ballNumber < 10)
				{
					al_draw_filled_circle(ball.getX(), ball.getY(), 5, al_map_rgb(255, 255, 255));
					al_draw_text(gameFont, al_map_rgb(0, 0, 0), ball.getX() - 3, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
				}
				else // draw double digit numbers
				{
					al_draw_text(gameFont, al_map_rgb(255, 255, 255), ball.getX() - 7, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
				}
			}
		}

		// handled here to give cue ball a higher z index,
		// so it appears over the other balls when it is ball in hand
		const Ball& cueBall{ gameBalls[0] };
		if (cueBall.isVisible())
		{
			al_draw_filled_circle(cueBall.getX(), cueBall.getY(), cueBall.getRadius(), al_map_rgb(255, 255, 255));
			al_draw_circle(cueBall.getX(), cueBall.getY(), cueBall.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);
		}
	}

	void drawPockets()
	{
		for (const std::vector<int>& coord : consts::pocketCoordinates)
		{
			al_draw_filled_circle(coord[0], coord[1], consts::pocketRadius, al_map_rgb(0, 0, 0));
		}
	}

	void drawCueStick(CueStick stick)
	{
		if (stick.isVisible())
		{
			al_draw_line(
				stick.m_outlineStartX,
				stick.m_outlineStartY,
				stick.m_outlineEndX,
				stick.m_outlineEndY,
				al_map_rgb(0, 0, 0),
				consts::cueStickThickness
			);

			al_draw_line(
				stick.m_stickStartX,
				stick.m_stickStartY,
				stick.m_stickEndX,
				stick.m_stickEndY,
				al_map_rgb(164, 116, 73),
				consts::cueStickWoodThickness
			);
		}
	}

	void drawPlaysurface()
	{
		al_clear_to_color(al_map_rgb(181, 101, 29));

		al_draw_filled_rectangle(
			consts::playSurface.xPos1,
			consts::playSurface.yPos1,
			consts::playSurface.xPos2,
			consts::playSurface.yPos2,
			al_map_rgb(0, 123, 0)
		);
	}

	// lol...it just makes the code more informative
	// much more sense to say renderDrawings than flip_display
	void renderDrawings()
	{
		al_flip_display();
	}
}
