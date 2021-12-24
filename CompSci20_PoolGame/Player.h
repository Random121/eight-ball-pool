#pragma once

#include "utilities.h"

class Player
{
private:
	BallType m_targetType{ BallType::undetermined };
	int m_gameScore{};

public:
	int getGameScore() const;
	void setGameScore(int score);
	void addGameScore(int score);

	BallType getTargetBallType() const;
	void setTargetBallType(BallType type);
};

