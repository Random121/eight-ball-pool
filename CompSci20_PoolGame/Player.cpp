#include "Player.h"

int Player::getGameScore() const
{
	return m_gameScore;
}

void Player::setGameScore(int score)
{
	m_gameScore = score;
}

void Player::addGameScore(int score)
{
	m_gameScore += score;
}

BallType Player::getTargetBallType() const
{
	return m_targetType;
}

void Player::setTargetBallType(BallType type)
{
	m_targetType = type;
}