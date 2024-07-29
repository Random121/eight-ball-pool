#pragma once

#include "Ball.h"
#include "common.h"
#include "Players.h"

#include <vector>
#include <string>

class Players
{
public:
	struct PlayerType
	{
		std::string name;
		int score{};
		Ball::BallSuitType targetBallType{};
	};

private:
	std::vector<PlayerType> m_gamePlayers;
	int m_currentPlayerIndex{};

public:
	Players(const int numberOfPlayers);

	int getNextIndex() const;
	void advancePlayerIndex();
	void setPlayerIndex(const int index);

	std::vector<PlayerType>& getPlayerVector();
	PlayerType& getCurrentPlayer();
	PlayerType& getNextPlayer();
	PlayerType& getPlayer(const int playerIndex);

	int getCurrentIndex() const;
	int getPlayerCount() const;
};
