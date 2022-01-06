#include "constants.h"
#include "common.h"
#include "Input.h"
#include "AllegroHandler.h"
#include "GameLogic.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

static void setPlayerNames(std::string& playerName1, std::string& playerName2)
{
	std::cout << "=========================\n";
	std::cout << "= Player Name Selection =\n";
	std::cout << "=========================\n\n";

	std::cout << "Enter name for Player (1): ";
	std::getline(std::cin, playerName1);

	if (playerName1.empty())
		playerName1 = "1";

	while (true)
	{
		std::cout << "Enter name for Player (2): ";
		std::getline(std::cin, playerName2);

		if (playerName2 == playerName1)
		{
			std::cout << "Players can not have the same name.\n";
		}
		else
		{
			if (playerName2.empty())
			{
				playerName2 = "2";
			}
			break;
		}
	}

	pauseProgram("\nPress [ENTER] to go back to main menu...");
}

static void displayCredits()
{
	std::cout << "===========\n";
	std::cout << "= Credits =\n";
	std::cout << "===========\n\n";

	std::cout << "[Allegro Game Library]\n";
	std::cout << "For providing this easy to use game and rendering library so I don't have to learn OpenGL.\n\n";

	std::cout << "[Learncpp]\n";
	std::cout << "Helping me with learning OOP and function templates.\n\n";

	std::cout << "[Stackoverflow Users (https://stackoverflow.com/a/6487534)]\n";
	std::cout << "For providing me with this better alternative to clear the console.\n\n";

	std::cout << "[Youtuber javidx9 (https://youtu.be/LPzyNOHY3A4)]\n";
	std::cout << "Intuitive explanation on the maths for the collision.\n\n";

	std::cout << "[Blog (https://lajbert.github.io/blog/fixed_timestep/#/)]\n";
	std::cout << "Explanations on why use and how to implement fixed time updates.\n\n";

	std::cout << "[freesound.org]\n";
	std::cout << "Sound effect samples for ball collisions and pocketing.\n\n";

	pauseProgram("Press [ENTER] to go back to main menu...");
}

static bool initMenu(std::string& playerName1, std::string& playerName2)
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
		std::cout << "[3] Credits\n";
		std::cout << "[4] Exit\n\n";

		std::cout << "Select Option: ";
		std::cin >> userSelection;

		resetCin();
		clearConsole();

		if (!std::cin.fail())
		{
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
			case 4:
				return true;
			}
		}

		clearConsole();
	}

	return false;
}

int main()
{
	{ // initialize random number gen
		std::srand(std::time(nullptr));
		const int temp{ std::rand() };
	}

	std::string playerName1{ "1" };
	std::string playerName2{ "2" };

	if (initMenu(playerName1, playerName2))
		return EXIT_SUCCESS;

	AllegroHandler allegro{};
	Input& input{ Input::getInstance() };
	GameLogic gameLogic{ allegro, playerName1, playerName2 };
	al_set_window_title(allegro.getDisplay(), "Totally Accurate Eight-Ball Simulator");

	bool gameRunning{ true };
	ALLEGRO_EVENT_TYPE eventType;
	allegro.startTimer();

#ifdef DISPLAY_FPS
	unsigned int frames{};
	double prevFrameStart{ al_get_time() };
	double currentFrameTime;
#endif // DISPLAY_FPS

	while (gameRunning)
	{
		al_wait_for_event(allegro.getEventQueue(), &allegro.getEvent());
		eventType = allegro.getEvent().type;

		if (eventType == ALLEGRO_EVENT_TIMER)
		{
			input.updateAllStates();
			gameLogic.frameUpdate(gameRunning);
			if (input.isKeyDown(ALLEGRO_KEY_ESCAPE))
			{
				gameRunning = false;
			}

#ifdef DISPLAY_FPS
			frames++;
			currentFrameTime = al_get_time();
			if (currentFrameTime - prevFrameStart > 1)
			{
				std::cout << "[FPS]: " << (frames / (currentFrameTime - prevFrameStart)) << '\n';
				prevFrameStart = currentFrameTime;
				frames = 0;
			}
#endif // DISPLAY_FPS
		}
		else if (eventType == ALLEGRO_EVENT_KEY_DOWN)
		{
			input.keyDownHook(allegro.getEvent().keyboard.keycode);
		}
		else if (eventType == ALLEGRO_EVENT_KEY_UP)
		{
			input.keyUpHook(allegro.getEvent().keyboard.keycode);
		}
		else if (eventType == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameRunning = false;
		}
	}

	allegro.destroyDisplay();
	pauseProgram("Thank you for playing. Press [ENTER] to exit...");

	return EXIT_SUCCESS;
}
