#pragma once

#include "Ball.h"
#include "common.h"
#include "Players.h"

#include <vector>
#include <string>

struct Player
{
	std::string name;
	int score{};
	BallType targetBallType{};
};

class Players
{
private:
	std::vector<Player> m_gamePlayers;
	int m_currentPlayerIndex{};

public:
	Players(const int numberOfPlayers);

	int getNextIndex() const;
	void advancePlayerIndex();
	void setPlayerIndex(const int index);

	std::vector<Player>& getPlayerVector();
	Player& getCurrentPlayer();
	Player& getNextPlayer();
	Player& getPlayer(const int playerIndex);

	int getCurrentIndex() const;
	int getPlayerCount() const;
};
