#include "GameEnvironment.h"
#include "Ball.h"

GameEnvironment::GameEnvironment()
{
}

GameEnvironment& GameEnvironment::getInstance()
{
	static GameEnvironment* m_instance{ new GameEnvironment() };
	return *m_instance;
}

std::vector<Ball>& GameEnvironment::getGameBalls()
{
	return m_gameBalls;
}

Ball& GameEnvironment::getCueBall()
{
	if (!m_gameBalls.empty())
		return m_gameBalls[0];
}

Ball& GameEnvironment::getEightBall()
{
	if (!m_gameBalls.empty())
		return m_gameBalls[7];
}

bool GameEnvironment::areBallsMoving() const
{
	for (const Ball& ball : m_gameBalls)
	{
		if (ball.isMoving())
		{
			return true;
		}
	}
	return false;
}