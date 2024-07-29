#include "Players.h"

Players::Players(const int numberOfPlayers)
{
	// also creates the player structs, nice
	m_gamePlayers.resize(numberOfPlayers);
}

int Players::getNextIndex() const
{
	return (m_currentPlayerIndex + 1) % m_gamePlayers.size();
}

void Players::advancePlayerIndex()
{
	m_currentPlayerIndex = getNextIndex();
}

void Players::setPlayerIndex(const int index)
{
	if (index >= 0 && index < m_gamePlayers.size())
	{
		m_currentPlayerIndex = index;
	}
}

std::vector<Players::PlayerType>& Players::getPlayerVector()
{
	return m_gamePlayers;
}

Players::PlayerType& Players::getCurrentPlayer()
{
	return m_gamePlayers[m_currentPlayerIndex];
}

Players::PlayerType& Players::getNextPlayer()
{
	return m_gamePlayers[getNextIndex()];
}

Players::PlayerType& Players::getPlayer(const int playerIndex)
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

int Players::getPlayerCount() const
{
	return static_cast<int>(m_gamePlayers.size());
}
