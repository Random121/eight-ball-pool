#include "render.h"
#include "Ball.h"
#include "constants.h"
#include "common.h"
#include "CueStick.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <iostream>
#include <string>
#include <array>

namespace render
{
	void drawBalls(const Ball::balls_type& gameBalls, ALLEGRO_FONT* const& gameFont)
	{
		for (const Ball& ball : gameBalls)
		{
			// skip rendering the cue ball and non visible balls
			if (ball.getBallNumber() == 0 || !ball.isVisible())
				continue;

			const int ballNumber{ ball.getBallNumber() };
			const Ball::BallSuitType type{ ball.getBallType() };
			const std::string& ballNumberString{ std::to_string(ballNumber) };

			// draw striped balls
			if (type == Ball::BallSuitType::striped)
			{
				// draw the white circle background
				al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(255, 255, 255));

				const auto& [red, green, blue] { consts::ballColorMap[ballNumber - 9] };
				al_draw_filled_circle(ball.getX(), ball.getY(), 11, al_map_rgb(red, green, blue));
			}
			else // draw solid balls including eight ball
			{
				const auto& [red, green, blue] { consts::ballColorMap[ballNumber - 1] };
				al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(red, green, blue));
			}

			// draw ball border
			al_draw_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);

			// draw single digit numbers
			if (ballNumber < 10)
			{
				al_draw_filled_circle(ball.getX(), ball.getY(), 5, al_map_rgb(255, 255, 255));
				al_draw_text(gameFont, al_map_rgb(0, 0, 0), ball.getX() - 3, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.data());
			}
			else // draw double digit numbers
			{
				al_draw_text(gameFont, al_map_rgb(255, 255, 255), ball.getX() - 7, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.data());
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
		for (const auto& [xCoord, yCoord] : consts::pocketCoordinates)
		{
			al_draw_filled_circle(xCoord, yCoord, consts::pocketRadius, al_map_rgb(0, 0, 0));
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
