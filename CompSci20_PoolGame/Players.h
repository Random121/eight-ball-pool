#pragma once

#include "Ball.h"
#include "utilities.h"
#include "Players.h"

#include <vector>
#include <string_view>

struct Player
{
	std::string_view name{};
	int score{};
	BallType targetBallType{};
};

class Players
{
private:
	std::vector<Player> m_gamePlayers;
	int m_currentPlayerIndex;

public:
	Players(const int numberOfPlayers, const int startIndex = 0);

	int getNextIndex() const;
	void advancePlayerIndex();

	std::vector<Player>& getPlayerVector();
	Player& getCurrentPlayer();
	Player& getNextPlayer();
	Player& getPlayer(const int playerIndex);

	int getCurrentIndex() const;
	int getCurrentIndexPretty() const;
	int getPlayerCount() const;
};
