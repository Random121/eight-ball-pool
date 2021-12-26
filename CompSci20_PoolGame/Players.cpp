#include "Players.h"

Players::Players(const int numberOfPlayers, const int startIndex)
{
	// also creates the player structs, nice
	m_gamePlayers.resize(numberOfPlayers);

	if (startIndex >= 0 && startIndex < m_gamePlayers.size())
	{
		m_currentPlayerIndex = startIndex;
	}
}

int Players::getNextIndex() const
{
	return (m_currentPlayerIndex + 1) % m_gamePlayers.size();
}

void Players::advancePlayerIndex()
{
	m_currentPlayerIndex = getNextIndex();
}

std::vector<Player>& Players::getPlayerVector()
{
	return m_gamePlayers;
}

Player& Players::getCurrentPlayer()
{
	return m_gamePlayers[m_currentPlayerIndex];
}

Player& Players::getNextPlayer()
{
	return m_gamePlayers[getNextIndex()];
}

Player& Players::getPlayer(const int playerIndex)
{
	if (playerIndex >= 0 && playerIndex < m_gamePlayers.size())
	{
		return m_gamePlayers[playerIndex];
	}
}

int Players::getCurrentIndex() const
{
	return m_currentPlayerIndex;
}

int Players::getCurrentIndexPretty() const
{
	return m_currentPlayerIndex + 1;
}

int Players::getPlayerCount() const
{
	return m_gamePlayers.size();
}