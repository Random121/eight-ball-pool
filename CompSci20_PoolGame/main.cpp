#include "constants.h"
#include "common.h"
#include "Input.h"
#include "AllegroHandler.h"
#include "GameLogic.h"
#include "menu.h"

#include <allegro5/allegro5.h>

#include <iostream>
#include <string>
#include <ctime>

int main()
{
	{ // initialize random number gen
		std::srand(std::time(nullptr));
		const int temp{ std::rand() };
	}

	// application lifetime variables
	AllegroHandler allegro{};
	Input& input{ Input::getInstance() };

	std::string playerName1{ "1" };
	std::string playerName2{ "2" };

	bool gameRunning;
	ALLEGRO_EVENT_TYPE eventType;

#ifdef DISPLAY_FPS
	unsigned int frames{};
	double prevFrameStart{ al_get_time() };
	double currentFrameTime;
#endif // DISPLAY_FPS

	// application loop
	while (true)
	{
		// display main menu
		if (menu::initMenu(playerName1, playerName2))
		{
			pauseProgram("Thank you for playing. Press [ENTER] to exit...");
			return EXIT_SUCCESS;
		}

		// setup game
		allegro.createDisplay();
		al_set_window_title(allegro.getDisplay(), "Totally Accurate Eight-Ball Simulator");

		GameLogic gameLogic{ allegro, playerName1, playerName2 };
		gameRunning = true;

		allegro.startTimer();

		// game loop
		while (gameRunning)
		{
			al_wait_for_event(allegro.getEventQueue(), &allegro.getEvent());
			eventType = allegro.getEvent().type;

			if (eventType == ALLEGRO_EVENT_TIMER)
			{
				input.updateAllStates();

				// return true if game has ended
				if (gameLogic.frameUpdate())
					break;

				if (input.isKeyDown(ALLEGRO_KEY_ESCAPE))
					break;

#ifdef DISPLAY_FPS
				frames++;
				currentFrameTime = al_get_time();
				if (currentFrameTime - prevFrameStart >= 1)
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
				break;
			}
		}

		allegro.stopTimer();
		allegro.destroyFont();
		allegro.destroyDisplay();

		clearConsole();
	}

	// this should never be run as the main menu should exit the program
	return EXIT_FAILURE;
}
