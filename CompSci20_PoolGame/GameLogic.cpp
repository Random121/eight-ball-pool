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

static void createBalls(Ball::balls_type& gameBalls, const int ballCount, const double ballRadius, const double ballMass)
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

static void setupRack(Ball::balls_type& gameBalls)
{
	static std::vector<int> ballIndexes{ 1, 2, 3, 4, 6, 7, 9, 10, 12, 13, 14, 15 };

	intArrayFisherYatesShuffle(ballIndexes);

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

static bool isValidPlacePosition(Ball& cueBall, Ball::balls_type& gameBalls)
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

GameLogic::GameLogic(AllegroHandler& allegro, const std::string& playerName1, const std::string& playerName2)
	: m_allegro{ allegro },
	m_gamePlayers{ 2 }
{
	createBalls(m_gameBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);
	setupRack(m_gameBalls);

	m_gamePlayers.getPlayer(0).name = playerName1;
	m_gamePlayers.getPlayer(1).name = playerName2;

	m_gamePlayers.setPlayerIndex(getRandomInteger(0, 1));

	std::cout << "[Breaker]: Player (" << m_gamePlayers.getCurrentPlayer().name << ")\n\n";
}

bool GameLogic::frameUpdate()
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

		m_gameCueStick.setCuePower(0);
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
				// game finished
				return true;
			}
		}
	}

	if (m_allegro.isEventQueueEmpty())
	{
		updateRender();
	}

	return false;
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

	// limit max frame time
	if (frameTime > 0.25)
		frameTime = 0.25;

	timeAccumulator += frameTime;

	while (timeAccumulator >= consts::physicsUpdateDelta)
	{
		physics::stepPhysics(m_gameBalls, m_gamePlayers, m_activeTurn, m_allegro);
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
		std::cout << "[Ball Shot] Power: " << cuePower << "\n\n";
	}
}

bool GameLogic::endTurn()
{
	clearConsole();

	const bool hasPocketedBall{ m_activeTurn.pocketedBalls.size() > 0 };
	const bool didFoul{ !referee::isTurnValid(m_gamePlayers.getCurrentPlayer(), m_activeTurn) };

	std::cout << "[Turn Over]: Player (" << m_gamePlayers.getCurrentPlayer().name << ")\n";
	std::cout << "Pocketed Balls: ";
	//std::cout << "First Hit Ball Type: " << getBallTypeName(m_activeTurn.firstHitBallType) << '\n';
	//std::cout << "Player Target Ball Type: " << getBallTypeName(m_gamePlayers.getCurrentPlayer().targetBallType) << '\n';

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
		std::cout << "None\n";
	}
	std::cout << '\n';

	// check and handle game overs
	if (referee::isGameFinished(m_gameBalls))
	{
		const std::string winnerName{ (!didFoul) ? m_gamePlayers.getCurrentPlayer().name : m_gamePlayers.getNextPlayer().name };

		std::cout << "[Winner]: Player (" << winnerName << ")\n\n";

		al_show_native_message_box(
			m_allegro.getDisplay(),
			"Match Winner",
			std::string("Congratulations! Player (" + winnerName + ") has won this Eight-Ball match.").c_str(),
			nullptr,
			nullptr,
			NULL
		);

		return true;
	}

	// announce the newly assigned suits
	if (m_activeTurn.targetBallsSelectedThisTurn)
	{
		std::cout << "[Ball Suits Have Been Assigned]\n";

		for (const Players::PlayerType& player : m_gamePlayers.getPlayerVector())
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
	if (hasPocketedBall && m_gamePlayers.getCurrentPlayer().targetBallType != Ball::BallSuitType::unknown)
	{
		for (Ball* pocketedBall : m_activeTurn.pocketedBalls)
		{
			for (Players::PlayerType& player : m_gamePlayers.getPlayerVector())
			{
				if (player.targetBallType == pocketedBall->getBallType())
				{
					player.score++;
				}
			}
		}
	}

	// print scores
	std::cout << "[Match Scores]\n";

	for (const Players::PlayerType& player : m_gamePlayers.getPlayerVector())
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

	std::cout << "[Turn Start]: Player (" << m_gamePlayers.getCurrentPlayer().name << ")\n";

	// tell the player that they have ball in hand
	if (didFoul)
	{
		std::cout << "[Ball In Hand]: Player (" << m_gamePlayers.getCurrentPlayer().name << ") is starting with ball in hand.\n";
	}

	std::cout << '\n';

	// remind the players of the current ball suit selections
	if (!m_activeTurn.targetBallsSelectedThisTurn && m_gamePlayers.getCurrentPlayer().targetBallType != Ball::BallSuitType::unknown)
	{
		std::cout << "[Current Ball Suit Assignments]\n";

		for (const Players::PlayerType& player : m_gamePlayers.getPlayerVector())
		{
			std::cout << "Player (" << player.name << ") is assigned " << getBallTypeName(player.targetBallType) << " balls.\n";
		}

		std::cout << '\n';
	}

	// reset all the turn information and set if the next player has ball in hand
	m_activeTurn = {};
	m_activeTurn.startWithBallInHand = didFoul;
}
