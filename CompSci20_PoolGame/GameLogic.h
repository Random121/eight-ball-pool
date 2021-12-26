#pragma once

#include "AllegroHandler.h"
#include "Input.h"
#include "Players.h"
#include "Ball.h"
#include "CueStick.h"

#include "Input.h"

#include <vector>

class GameLogic
{
public:
	using BallVector = std::vector<Ball>;
private:
	// shared resources
	AllegroHandler& m_allegro;
	Input& m_input{ Input::getInstance() };

	Players m_gamePlayers;
	BallVector m_gameBalls;

	CueStick m_gameCueStick{ true, true };
	TurnInformation m_activeTurn{};

	double m_lastShotStartTime{};

public:

	GameLogic(AllegroHandler& allegro);

	void shootCueBall();

	void updatePhysics();
	void updateRender();
	void frameUpdate();
};

