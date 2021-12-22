#include "render.h"
#include "Ball.h"
#include "constants.h"
#include "utilities.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <string>
#include <vector>
#include <cmath>

namespace render
{
	void drawBalls(const std::vector<Ball>& gameBalls, ALLEGRO_FONT*& gameFont)
	{
		for (int ballNumber{}; ballNumber < gameBalls.size(); ++ballNumber)
		{
			const Ball& ball{ gameBalls[ballNumber] };

			if (!ball.isVisible()) // skip if not visible
				continue;

			if (ballNumber == 0 || ballNumber > 8) // these balls need a white circle
			{
				// draw white circle
				al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(255, 255, 255));

				if (ballNumber > 8) // draw center color for balls
				{
					const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 9] };
					al_draw_filled_circle(ball.getX(), ball.getY(), 11, al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
				}
			}
			else
			{
				const std::vector<int>& rgbValues{ consts::ballColorMap[ballNumber - 1] };
				al_draw_filled_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(rgbValues[0], rgbValues[1], rgbValues[2]));
			}

			//draw ball border
			al_draw_circle(ball.getX(), ball.getY(), ball.getRadius(), al_map_rgb(0, 0, 0), consts::ballBorderThickness);

			if (ballNumber != 0) // do not write ball number on cue
			{
				const std::string ballNumberString{ std::to_string(ballNumber) };
				if (ballNumber < 10)
				{
					al_draw_filled_circle(ball.getX(), ball.getY(), 5, al_map_rgb(255, 255, 255));
					al_draw_text(gameFont, al_map_rgb(0, 0, 0), ball.getX() - 3, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
				}
				else
				{
					al_draw_text(gameFont, al_map_rgb(255, 255, 255), ball.getX() - 7, ball.getY() - 4, ALLEGRO_ALIGN_LEFT, ballNumberString.c_str());
				}
			}
		}
	}

	void drawPockets()
	{
		for (const std::vector<int>& coord : consts::pocketCoordinates)
		{
			al_draw_filled_circle(coord[0], coord[1], consts::pocketRadius, al_map_rgb(0, 0, 0));
		}
	}

	void drawCueStick(const Ball& cueBall, const ALLEGRO_MOUSE_STATE& mouseState, const double stickPower)
	{
		double cueDistanceFromBall{ consts::cueStickDistanceFromBall + stickPower };
		double cueStickLength{ consts::cueStickLength + stickPower };

		const double deltaX{ cueBall.getX() - mouseState.x };
		const double deltaY{ cueBall.getY() - mouseState.y };
		const double cursorBallDistance{ calculateHypotenuse(deltaX, deltaY) };

		const double vectorShrinkFactor{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / (cueDistanceFromBall * cueDistanceFromBall)) };
		const double vectorShrinkFactorWood{ std::sqrt((deltaX * deltaX + deltaY * deltaY) / ((cueDistanceFromBall + 4) * (cueDistanceFromBall + 4))) };

		al_draw_line(
			cueBall.getX() + (deltaX / vectorShrinkFactor), // cue start x
			cueBall.getY() + (deltaY / vectorShrinkFactor), // cue start y
			cueBall.getX() + (deltaX / cursorBallDistance) * cueStickLength, // cue end x
			cueBall.getY() + (deltaY / cursorBallDistance) * cueStickLength, // cue end y
			al_map_rgb(0, 0, 0),
			consts::cueStickThickness
		);

		al_draw_line(
			cueBall.getX() + (deltaX / vectorShrinkFactorWood), // cue wood start x
			cueBall.getY() + (deltaY / vectorShrinkFactorWood), // cue wood start y
			cueBall.getX() + (deltaX / cursorBallDistance) * (cueStickLength - 2), // cue wood end x
			cueBall.getY() + (deltaY / cursorBallDistance) * (cueStickLength - 2), // cue wood end y
			al_map_rgb(164, 116, 73),
			consts::cueStickWoodThickness
		);
	}
		
	void drawPlaysurface()
	{
		al_clear_to_color(al_map_rgb(181, 101, 29));

		al_draw_filled_rectangle(
			consts::playSurface.xPos1,
			consts::playSurface.yPos1,
			consts::playSurface.xPos2,
			consts::playSurface.yPos2,
			al_map_rgb(0, 110, 0)
		);
	}

	//void renderFrame(const std::vector<Ball>& gameBalls, const bool isMouseDown, const ALLEGRO_MOUSE_STATE& mouseState, ALLEGRO_FONT*& gameFont, const bool allBallsStopped, const double stickPower)
	//{
	//	al_clear_to_color(al_map_rgb(181, 101, 29));

	//	// draw play surface
	//	al_draw_filled_rectangle(
	//		consts::playSurface.xPos1,
	//		consts::playSurface.yPos1,
	//		consts::playSurface.xPos2,
	//		consts::playSurface.yPos2,
	//		al_map_rgb(0, 110, 0)
	//	);

	//	drawPockets();
	//	drawBalls(gameBalls);

	//	if (allBallsStopped)
	//	{
	//		drawCueStick(gameBalls[0], mouseState, stickPower);
	//		if (isMouseDown)
	//			al_draw_line(mouseState.x, mouseState.y, gameBalls[0].getX(), gameBalls[0].getY(), al_map_rgb(0, 0, 255), 3);
	//	}


	//	al_flip_display();
	//}
}