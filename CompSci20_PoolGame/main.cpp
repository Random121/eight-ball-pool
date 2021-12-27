#include "constants.h"
#include "common.h"
#include "Input.h"
#include "AllegroHandler.h"
#include "GameLogic.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <ctime>
#include <limits>

/*
void updatePhysics(std::vector<Ball>& gameBalls, const double updateDelta, Players& gamePlayers, TurnInformation& turnInfo)
{
	static double previousTime{ al_get_time()};
	static double currentTime{};
	static double frameTime{};
	static double timeAccumulator{};

	currentTime = al_get_time();
	frameTime = currentTime - previousTime;
	previousTime = currentTime;

	// max frame time
	if (frameTime > 0.25)
		frameTime = 0.25;

	timeAccumulator += frameTime;

#ifdef DEBUG
	std::cout << "[FRAME TIME] " << frameTime << '\n';
#endif // DEBUG

	while (timeAccumulator >= updateDelta)
	{
		physics::stepPhysics(gameBalls, gamePlayers, turnInfo);
		timeAccumulator -= updateDelta;
	}
}
*/

/*
void shootCueBall(Ball& cueBall, CueStick& cueStick, Input& input)
{
	const int power{ cueStick.getCuePower() };
	if (power > 0)
	{
		const double deltaX{ input.getMouseX() - cueBall.getX() };
		const double deltaY{ input.getMouseY() - cueBall.getY() };
		const double hyp{ calculateHypotenuse(deltaX, deltaY) }; // for normalization

		cueBall.setVelocity(
			(deltaX / hyp) * power,
			(deltaY / hyp) * power
		);

		std::cout << "Cue Stick Power: " << power << '\n';

		// reset the cue stick position to make it seem like
		// it has been "shot"
		cueStick.setCuePower(0);
		cueStick.updateAll(cueBall.getX(), cueBall.getY());
		cueStick.setCanUpdate(false);
	}
}

bool isValidPlacePosition(Ball& cueBall, std::vector<Ball>& gameBalls)
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

*/

void setPlayerNames(std::string_view& playerName1, std::string_view& playerName2)
{
	std::cout << "=========================\n";
	std::cout << "= Player Name Selection =\n";
	std::cout << "=========================\n\n";
	
	std::string temp;

	std::cout << "Enter name for Player (1): ";
	std::getline(std::cin, temp);

	playerName1 = temp;

	std::cout << "Enter name for Player (2): ";
	std::getline(std::cin, temp);

	playerName2 = temp;

	pauseProgram("\nPress [ENTER] to go back to main menu...");
}

void displayCredits()
{
	std::cout << "===========\n";
	std::cout << "= Credits =\n";
	std::cout << "===========\n\n";

	std::cout << "Allegro | For providing this easy to use game and rendering library so I don't have to learn OpenGL.\n\n";
	std::cout << "Learncpp | Helping me with learning OOP and for providing a better random number generator function.\n\n";
	std::cout << "Random Math Websites | Teaching how to do vector maths.\n\n";
	std::cout << "Stackoverflow Users (https://stackoverflow.com/a/6487534) | For providing me with this better alternative to clear the console.\n\n";
	std::cout << "Youtuber javidx9 (https://www.youtube.com/watch?v=LPzyNOHY3A4) | Intuitive explanation on how to calculate the collision responses between two circles.\n\n";
	std::cout << "This Blog (https://lajbert.github.io/blog/fixed_timestep/#/) - Explanations on why use and how to implement fixed time updates.\n\n";

	pauseProgram("Press [ENTER] to go back to main menu...");
}

void initMenu(std::string_view& playerName1, std::string_view& playerName2)
{
	bool menuActive{ true };
	int userSelection;

	while (menuActive)
	{

		std::cout << "====================================================\n";
		std::cout << "= Welcome to Totally Accurate Eight-Ball Simulator =\n";
		std::cout << "====================================================\n\n";

		std::cout << "===Please select one of the options below===\n";
		std::cout << "[1] Play Eight-Ball\n";
		std::cout << "[2] Setup Player Names\n";
		std::cout << "[3] Credits\n\n";
	
		std::cout << "Select Option: ";
		std::cin >> userSelection;

		if (std::cin.fail())
		{
			resetCin();
			clearConsole();
			continue;
		}

		resetCin();
		clearConsole();

		switch (userSelection)
		{
		case 1:
			menuActive = false;
			break;
		case 2:
			setPlayerNames(playerName1, playerName2);
			break;
		case 3:
			displayCredits();
			break;
		}

		clearConsole();
	}
}

int main()
{
	{ // initialize random number gen
		std::srand(std::time(nullptr));
		const int temp{ std::rand() };
	}

	std::string_view playerName1{};
	std::string_view playerName2{};

	initMenu(playerName1, playerName2);

	AllegroHandler allegro{};
	Input& input{ Input::getInstance() };
	GameLogic gameLogic{ allegro, playerName1, playerName2 };

	al_set_window_title(allegro.getDisplay(), "Totally Accurate Eight-Ball Simulator");

	//constexpr int gamePlayerCount{ 2 };

	//Players gamePlayers{ gamePlayerCount, getRandomInteger(0, gamePlayerCount - 1) };
	//std::vector<Ball> gameBalls;
	//CueStick gameStick{ true, true };
	//TurnInformation currentTurn;

	bool gameRunning{ true };
	//bool drawFrame{ true };
	//int shootStartTime{};

	//al_show_native_message_box(
	//	gameDisplay,
	//	"Break Shot",
	//	"Player taking the break is: ",
	//	std::to_string(currentPlayerIndex + 1).c_str(),
	//	nullptr,
	//	NULL
	//);

	//createBalls(gameBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);
	//moveToRackPositions(gameBalls);

	//std::cout << "Player (" << gamePlayers.getCurrentIndexPretty() << ") is taking the break shot.\n";

	allegro.startTimer();

	while (gameRunning)
	{
		al_wait_for_event(allegro.getEventQueue(), &allegro.getEvent());

		switch (allegro.getEvent().type)
		{
		case ALLEGRO_EVENT_TIMER:

			input.updateAllStates();

			gameLogic.frameUpdate(gameRunning);

			if (input.isKeyDown(ALLEGRO_KEY_ESCAPE))
				gameRunning = false;

			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			input.keyDownHook(allegro.getEvent().keyboard.keycode);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			input.keyUpHook(allegro.getEvent().keyboard.keycode);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		}

		/*
		switch (allegro.getEvent().type)
		{
		case ALLEGRO_EVENT_TIMER:
			updatePhysics(gameBalls, consts::physicsUpdateDelta, gamePlayers, currentTurn);
			input.updateAllStates();
			gameStick.updateAll(gameBalls[0].getX(), gameBalls[0].getY());

			//if (input.isKeyDown(ALLEGRO_KEY_W))
			//	gameBalls[0].addPosition(0, -1);
			//if (input.isKeyDown(ALLEGRO_KEY_S))
			//	gameBalls[0].addPosition(0, 1);
			//if (input.isKeyDown(ALLEGRO_KEY_A))
			//	gameBalls[0].addPosition(-1, 0);
			//if (input.isKeyDown(ALLEGRO_KEY_D))
			//	gameBalls[0].addPosition(1, 0);

			//if (input.isMouseButtonDown(1))
			//{
			//	shootCueBall(gameBalls[0], gameStick, input);
			//	shootStartTime = al_get_time();
			//}
			//else if (!gameStick.getCanUpdate() && al_get_time() - shootStartTime > 2)
			//{
			//	gameStick.setVisible(false);
			//}

			if (gameStick.canUpdate() && input.isMouseButtonDown(1))
			{
				if (currentTurn.startWithBallInHand)
				{
					gameBalls[0].setPosition(input.getMouseX(), input.getMouseY());

					if (isValidPlacePosition(gameBalls[0], gameBalls))
					{
						gameBalls[0].setVisible(true);
						gameStick.setVisible(true);
						currentTurn.startWithBallInHand = false;
					}
				}
				else
				{
					shootCueBall(gameBalls[0], gameStick, input);
					shootStartTime = al_get_time();
				}
			}
			
			if (!gameStick.canUpdate() && al_get_time() - shootStartTime > 1)
			{
				gameStick.setVisible(false);
			}

			if (!physics::areBallsMoving(gameBalls) && !gameStick.isVisible() && !currentTurn.startWithBallInHand) //  turn finished
			{
				std::cout << "--TURN OVER--\n";

				std::cout << "First Hit Type: " << static_cast<int>(currentTurn.firstHitBallType) << '\n';
				std::cout << "Player Index: " << gamePlayers.getCurrentIndex() << '\n';
				//std::cout << "Valid Turn: " << currentTurn.isTurnValid << '\n';
				std::cout << "Player Ball Type: " << static_cast<int>(gamePlayers.getCurrentPlayer().targetBallType) << '\n';
				for (Ball* ball : currentTurn.pocketedBalls)
				{
					std::cout << ball->getBallNumber() << '\n';
				}

				const bool hasFouled = !referee::isTurnValid(gamePlayers.getCurrentPlayer(), currentTurn);

				if (gamePlayers.getCurrentPlayer().targetBallType != BallType::unknown)
				{
					for (Ball* ball : currentTurn.pocketedBalls)
					{
						if (ball->getBallType() == gamePlayers.getCurrentPlayer().targetBallType)
							gamePlayers.getCurrentPlayer().score++;
						else
							gamePlayers.getNextPlayer().score++;
					}
				}

				// check winner
				for (int i{}; i < gamePlayers.getPlayerCount(); ++i)
				{
					if (gamePlayers.getPlayer(i).score == 8)
					{
						std::cout << "[WINNER] Player (" << (i + 1) << ")\n";
						gameRunning = false;
						continue;
					}
				}

				std::cout << "FOULED: " << hasFouled << '\n';

				// switch turns
				if (hasFouled || currentTurn.pocketedBalls.size() == 0)
				{
					gamePlayers.advancePlayerIndex();
				}

				gameStick.setCanUpdate(true);

				if (!hasFouled)
					gameStick.setVisible(true);
				else
					gameBalls[0].setVisible(false);

				currentTurn = {};
				currentTurn.startWithBallInHand = hasFouled;

				std::cout << "SCORES: " << gamePlayers.getPlayer(0).score << ' ' << gamePlayers.getPlayer(1).score << '\n';
				std::cout << "Current Player: " << gamePlayers.getCurrentIndexPretty() << '\n';
				if (hasFouled)
					std::cout << "Current player is starting with ball in hand.\n";
			}

			if (input.isKeyDown(ALLEGRO_KEY_ESCAPE))
				gameRunning = false;

			drawFrame = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			input.keyDownHook(allegro.getEvent().keyboard.keycode);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			input.keyUpHook(allegro.getEvent().keyboard.keycode);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		}
		*/

		/*
		if (gameRunning && drawFrame && allegro.isEventQueueEmpty())
		{
			render::drawPlaysurface();
			render::drawPockets();
			render::drawBalls(gameBalls, allegro.getFont());
			render::drawCueStick(gameStick);
			render::renderDrawings();
			drawFrame = false;
		}
		*/
	}

	// freeing all the resources
	//al_destroy_event_queue(eventQueue);
	//al_destroy_timer(gameTimer);
	//al_destroy_display(gameDisplay);
	//al_destroy_font(gameFont);

	pauseProgram("Thank you for playing. Press [ENTER] to exit...");

	return EXIT_SUCCESS;
}
