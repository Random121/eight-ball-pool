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

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, const ALLEGRO_FONT* const& gameFont)
	{
		for (const Ball& ball : gameBalls)
		{
			if (ball.isVisible())
			{
				const int ballNumber{ ball.getBallNumber() };
				const BallType type{ ball.getBallType() };

				// striped and cue balls require white circle
				if (type == BallType::cue || type == BallType::striped)
				{
					al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(255, 255, 255));

					// striped ball require color
					if (type == BallType::striped)
					{
						const std::vector<int>& rgbValues{ consts::ballColorMap[ball.getBallNumber() - 9] };
						al_draw_filled_circle(ball.getX(), ball.getY(), 11, al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
					}
				}
				else // solid balls
				{
					const std::vector<int>& rgbValues{ consts::ballColorMap[ball.getBallNumber() - 1] };
					al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
				}

				// draw ball border
				al_draw_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);

				if (type != BallType::cue)
				{
					const std::string ballNumberString{ std::to_string(ballNumber) };
					if (ballNumber < 10)
					{
						// single digit numbers
						al_draw_filled_circle(ball.getX(), ball.getY(), 5, al_map_rgb(255, 255, 255));
						al_draw_text(gameFont, al_map_rgb(0, 0, 0), ball.getX() - 3, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
					}
					else
					{
						// double digit numbers
						al_draw_text(gameFont, al_map_rgb(255, 255, 255), ball.getX() - 7, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
					}
				}
			}
		}

#ifdef OLD_CODE
		//for (int ballNumber{}; ballNumber < gameBalls.size(); ++ballNumber)
		//{
		//	const Ball& ball{ gameBalls[ballNumber] };

		//	if (!ball.isVisible()) // skip if not visible
		//		continue;

		//	if (ballNumber == 0 || ballNumber > 8) // these balls need a white circle
		//	{
		//		// draw white circle
		//		al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(255, 255, 255));

		//		if (ballNumber > 8) // draw center color for balls
		//		{
		//			const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 9] };
		//			al_draw_filled_circle(ball.getX(), ball.getY(), 11, al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
		//		}
		//	}
		//	else
		//	{
		//		const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 1] };
		//		al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
		//	}

		//	//draw ball border
		//	al_draw_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);

		//	if (ballNumber != 0) // do not write ball number on cue
		//	{
		//		const std::string ballNumberString{ std::to_string(ballNumber) };
		//		if (ballNumber < 10)
		//		{
		//			al_draw_filled_circle(ball.getX(), ball.getY(), 5, al_map_rgb(255, 255, 255));
		//			al_draw_text(gameFont, al_map_rgb(0, 0, 0), ball.getX() - 3, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
		//		}
		//		else
		//		{
		//			al_draw_text(gameFont, al_map_rgb(255, 255, 255), ball.getX() - 7, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
		//		}
		//	}
		//}
#endif // OLD_CODE
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
#ifdef OLD_CODE
		//double cueDistanceFromBall{ consts::cueStickDistanceFromBall + stickPower };
		//double cueStickLength{ consts::cueStickLength + stickPower };

		//const double deltaX{ xPos - mouseState.x };
		//const double deltaY{ yPos - mouseState.y };
		//const double cursorBallDistance{ calculateHypotenuse(deltaX, deltaY) };

		//const double vectorShrinkFactor{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / (cueDistanceFromBall * cueDistanceFromBall)) };
		//const double vectorShrinkFactorWood{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / ((cueDistanceFromBall + 4) * (cueDistanceFromBall + 4))) };

		//al_draw_line(
		//	xPos + (deltaX / vectorShrinkFactor), // cue start x
		//	yPos + (deltaY / vectorShrinkFactor), // cue start y
		//	xPos + (deltaX / cursorBallDistance) * cueStickLength, // cue end x
		//	yPos + (deltaY / cursorBallDistance) * cueStickLength, // cue end y
		//	al_map_rgb(0, 0, 0),
		//	consts::cueStickThickness
		//);

		//al_draw_line(
		//	xPos + (deltaX / vectorShrinkFactorWood), // cue wood start x
		//	yPos + (deltaY / vectorShrinkFactorWood), // cue wood start y
		//	xPos + (deltaX / cursorBallDistance) * (cueStickLength - 2), // cue wood end x
		//	yPos + (deltaY / cursorBallDistance) * (cueStickLength - 2), // cue wood end y
		//	al_map_rgb(164, 116, 73),
		//	consts::cueStickWoodThickness
		//);
#endif // OLD_CODE

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