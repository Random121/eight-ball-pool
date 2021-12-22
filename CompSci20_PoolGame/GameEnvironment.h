#pragma once

#include "Ball.h"

#include <allegro5/allegro_font.h>

#include <vector>

class GameEnvironment
{
private:
	// prevent the class from being created of copied
	GameEnvironment();
	GameEnvironment(const GameEnvironment&) = delete;

	std::vector<Ball> m_gameBalls;
	ALLEGRO_FONT* gameFont;
	
public:
	static GameEnvironment& getInstance();

	std::vector<Ball>& getGameBalls();

	Ball& getCueBall();
	Ball& getEightBall();

	bool areBallsMoving() const;

};


