#include "GameLogic.h"

#include "AllegroHandler.h"
#include "Ball.h"
#include "Vector2.h"

#include "constants.h"
#include "common.h"
#include "render.h"
#include "referee.h"
#include "physics.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <random>
#include <algorithm>

static void createBalls(GameLogic::BallVector& gameBalls, const int ballCount, const double ballRadius, const double ballMass)
{
	gameBalls.resize(ballCount);
	for (int i{}; i < ballCount; ++i)
	{
		Ball& ball{ gameBalls[i] };
		ball.setRadius(ballRadius);
		ball.setMass(ballMass);
		ball.setBallNumber(i);
		ball.setVisible(true);
	}
}

static void setupRack(GameLogic::BallVector& gameBalls)
{
	static std::vector<int> ballIndexes{ 1, 2, 3, 4, 6, 7, 9, 10, 12, 13, 14, 15 };
	static std::random_device randomDevice;
	static std::mt19937 engine{ randomDevice() };
	
	std::shuffle(ballIndexes.begin(), ballIndexes.end(), engine);

	int ballIndex{};

	for (int rackIndex{ 1 }; rackIndex < consts::rackBallPositions.size(); ++rackIndex)
	{
		if (rackIndex == 8 || rackIndex == 11 || rackIndex == 5)
		{
			++rackIndex;
		}
		gameBalls[ballIndexes[ballIndex]].setPosition(consts::rackBallPositions[rackIndex][0], consts::rackBallPositions[rackIndex][1]);
		++ballIndex;
	}

	// cue and eight ball have constant rack position
	gameBalls[0].setPosition(consts::rackBallPositions[0][0], consts::rackBallPositions[0][1]);
	gameBalls[8].setPosition(consts::rackBallPositions[8][0], consts::rackBallPositions[8][1]);

	// back corners of rack should be of balls from different suits
	gameBalls[5].setPosition(consts::rackBallPositions[5][0], consts::rackBallPositions[5][1]);
	gameBalls[11].setPosition(consts::rackBallPositions[11][0], consts::rackBallPositions[11][1]);
}

static bool isValidPlacePosition(Ball& cueBall, GameLogic::BallVector& gameBalls)
{
	bool isOverlappingBall{};
	bool isOverlappingBoundary{};

	for (Ball& ball : gameBalls)
	{
		if (cueBall.isOverlappingBall(ball))
		{
			isOverlappingBall = true;
			break;
		}
	}

	isOverlappingBoundary = physics::isCircleCollidingWithBoundaryTop(cueBall, consts::playSurface)
		|| physics::isCircleCollidingWithBoundaryBottom(cueBall, consts::playSurface)
		|| physics::isCircleCollidingWithBoundaryLeft(cueBall, consts::playSurface)
		|| physics::isCircleCollidingWithBoundaryRight(cueBall, consts::playSurface);

	return !isOverlappingBall && !isOverlappingBoundary;
}

static std::string_view getBallTypeName(BallType type)
{
	switch (type)
	{
	case BallType::unknown:
		return "Unknown";
	case BallType::solid:
		return "Solid";
	case BallType::striped:
		return "Striped";
	case BallType::eight:
		return "Eight-Ball";
	case BallType::cue:
		return "Cue-Ball";
	default:
		return "???";
	}
}

GameLogic::GameLogic(AllegroHandler& allegro, const std::string& playerName1, const std::string& playerName2)
	: m_allegro{ allegro },
	m_gamePlayers{ 2 }
{
	createBalls(m_gameBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);
	setupRack(m_gameBalls);

	m_gamePlayers.getPlayer(0).name = playerName1;
	m_gamePlayers.getPlayer(1).name = playerName2;

	m_gamePlayers.setPlayerIndex(getRandomInteger(0, 1));

	std::cout << "[BREAKER | Player (" << m_gamePlayers.getCurrentPlayer().name << ")]\n\n";
}

void GameLogic::frameUpdate(bool& gameRunning)
{
	if (m_activeTurn.startWithBallInHand)
	{
		if (!m_gameBalls[0].isVisible())
		{
			m_gameBalls[0].setVisible(true);
			m_gameBalls[0].setVelocity(0, 0);
		}

		m_gameBalls[0].setPosition(m_input.getMouseVector());

		if (m_input.isMouseButtonDown(1) && isValidPlacePosition(m_gameBalls[0], m_gameBalls))
		{
			m_gameCueStick.setCanUpdate(true);
			m_gameCueStick.setVisible(true);
			m_gameBalls[0].setVisible(true);
			m_activeTurn.startWithBallInHand = false;
		}
	}
	else
	{
		updatePhysics();
		m_gameCueStick.updateAll(m_gameBalls[0].getX(), m_gameBalls[0].getY());

		if (m_gameCueStick.canUpdate() && m_input.isMouseButtonDown(1))
		{
			shootCueBall();
			m_lastShotStartTime = al_get_time();
		}

		// yeah...the canUpdate variable and checks is just for this nice to have animation
		if (!m_gameCueStick.canUpdate() && m_gameCueStick.isVisible() && (al_get_time() - m_lastShotStartTime) > 1)
		{
			m_gameCueStick.setVisible(false);
		}

		// if no balls are moving and cue stick is not visible (meaning it has shot a ball)
		// we interpret it as the end of the turn
		if (!physics::areBallsMoving(m_gameBalls) && !m_gameCueStick.isVisible())
		{
			if (endTurn())
			{
				gameRunning = false;
			}
		}
	}

	if (m_allegro.isEventQueueEmpty())
	{
		updateRender();
	}
}

// when running physics calculations we use a fixed delta time update,
// this ensures that no matter how fast or slow a frame takes the physics
// calculations is always accurate at that certain point in time.
// i.e. if one instance has frame time of 2 seconds and another with 2 miliseconds,
// after 4 seconds in real life, the calculated physics velocity and positions should
// be exactly the same.
void GameLogic::updatePhysics()
{
	static double timeAccumulator{};
	static double previousTime{ al_get_time() };
	static double currentTime;
	static double frameTime;

	currentTime = al_get_time();
	frameTime = currentTime - previousTime;
	previousTime = currentTime;

	if (frameTime > 0.25)
		frameTime = 0.25;

	timeAccumulator += frameTime;

	while (timeAccumulator >= consts::physicsUpdateDelta)
	{
		physics::stepPhysics(m_gameBalls, m_gamePlayers, m_activeTurn);
		timeAccumulator -= consts::physicsUpdateDelta;
	}
}

void GameLogic::updateRender()
{
	render::drawPlaysurface();
	render::drawPockets();
	render::drawBalls(m_gameBalls, m_allegro.getFont());
	render::drawCueStick(m_gameCueStick);
	render::renderDrawings();
}

void GameLogic::shootCueBall()
{
	const int cuePower{ m_gameCueStick.getCuePower() };
	if (cuePower > 0)
	{
		Ball& cueBall{ m_gameBalls[0] };
		const Vector2 deltaPosition{ m_input.getMouseVector().copyAndSubtract(cueBall.getPositionVector()) };
		Vector2 normalized{ deltaPosition.getNormalized() };
		normalized.multiply(cuePower);

		// reset the cue stick position to make it seem like
		// it has been "shot"
		m_gameCueStick.setCuePower(0);
		m_gameCueStick.updateAll(cueBall.getX(), cueBall.getY());
		m_gameCueStick.setCanUpdate(false);

		cueBall.setVelocity(normalized);
		std::cout << "[BALL SHOT] Power: " << cuePower << "\n\n";
	}
}

bool GameLogic::endTurn()
{
	const bool hasPocketedBall{ m_activeTurn.pocketedBalls.size() > 0 };
	const bool didFoul{ !referee::isTurnValid(m_gamePlayers.getCurrentPlayer(), m_activeTurn) };

	std::cout << "[TURN OVER | Player (" << m_gamePlayers.getCurrentPlayer().name << ")]\n";
	std::cout << "First Hit Ball Type: " << getBallTypeName(m_activeTurn.firstHitBallType) << '\n';
	std::cout << "Player Target Ball Type: " << getBallTypeName(m_gamePlayers.getCurrentPlayer().targetBallType) << '\n';
	std::cout << "Pocketed Balls:";

	// print pocketed balls
	if (hasPocketedBall)
	{
		std::cout << '\n';
		for (Ball* ball : m_activeTurn.pocketedBalls)
		{
			std::cout << "- " << ball->getBallNumber() << " (" << getBallTypeName(ball->getBallType()) << ")\n";
		}
	}
	else
	{
		std::cout << " None\n";
	}

	std::cout << '\n';

	if (referee::isGameFinished(m_gameBalls))
	{
		const std::string winnerName{ (!didFoul) ? m_gamePlayers.getCurrentPlayer().name : m_gamePlayers.getNextPlayer().name };

		std::cout << "[WINNER]: Player (" << winnerName << ")\n\n";

		al_show_native_message_box(
			m_allegro.getDisplay(),
			"GAME WINNER",
			std::string("Congratulations! Player (" + winnerName + ") has won this Eight-Ball match.").c_str(),
			nullptr,
			nullptr,
			NULL
		);

		return true;
	}
	
	if (m_activeTurn.isTargetBallsSelectedThisTurn)
	{
		std::cout << "[BALL SELECTIONS HAVE BEEN MADE]\n";

		for (const Player& player : m_gamePlayers.getPlayerVector())
		{
			std::cout << "Player (" << player.name << ") is assigned " << getBallTypeName(player.targetBallType) << " balls.\n";
		}

		std::cout << '\n';
	}

	m_gameBalls[0].setVisible(false);

	// make everything re-appear if not ball in hand
	if (!didFoul)
	{
		m_gameCueStick.setCanUpdate(true);
		m_gameCueStick.setVisible(true);
		m_gameBalls[0].setVisible(true);
	}

	// add scores
	if (hasPocketedBall && m_gamePlayers.getCurrentPlayer().targetBallType != BallType::unknown)
	{
		for (Ball* pocketedBall : m_activeTurn.pocketedBalls)
		{
			for (Player& player : m_gamePlayers.getPlayerVector())
			{
				if (player.targetBallType == pocketedBall->getBallType())
				{
					player.score++;
				}
			}
		}
	}

	// print scores
	std::cout << "[GAME SCORE]\n";

	for (const Player& player : m_gamePlayers.getPlayerVector())
	{
		std::cout << "Player (" << player.name << "): " << player.score << '\n';
	}

	std::cout << '\n';

	nextTurn(didFoul, hasPocketedBall);
	return false;
}

void GameLogic::nextTurn(const bool didFoul, const bool hasPocketedBall)
{
	// switch turns
	if (didFoul || !hasPocketedBall)
	{
		m_gamePlayers.advancePlayerIndex();
	}

	std::cout << "[TURN START | Player (" << m_gamePlayers.getCurrentPlayer().name << ")]\n";
	if (didFoul)
	{
		std::cout << "[Player (" << m_gamePlayers.getCurrentPlayer().name << ") is starting with ball in hand]\n";
	}
	std::cout << '\n';

	m_activeTurn = {};
	m_activeTurn.startWithBallInHand = didFoul;
}