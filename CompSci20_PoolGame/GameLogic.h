#pragma once

#include "AllegroHandler.h"
#include "Input.h"
#include "Players.h"
#include "Ball.h"
#include "CueStick.h"

#include "Input.h"

#include <vector>
#include <string>

class GameLogic
{
private:
	// shared resources
	AllegroHandler& m_allegro;
	Input& m_input{ Input::getInstance() };

	Players m_gamePlayers;
	Ball::balls_type m_gameBalls;

	CueStick m_gameCueStick{ true, true };
	TurnInformation m_activeTurn{};

	double m_lastShotStartTime{};

public:
	GameLogic(AllegroHandler& allegro, const std::string& playerName1, const std::string& playerName2);

	bool endTurn();
	void nextTurn(const bool didFoul, const bool hasPocketedBall);
	void shootCueBall();

	void updatePhysics();
	void updateRender();
	bool frameUpdate();
};
