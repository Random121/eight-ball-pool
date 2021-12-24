#include "Ball.h"
#include "constants.h"
#include "utilities.h"
#include "physics.h"
#include "render.h"
#include "Input.h"
#include "Player.h"
#include "CueStick.h"
#include "referee.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

void initializeLibraries()
{
	// random
	std::srand(std::time(nullptr));
	{ const int temp{ std::rand() }; } // for better randomization

	// allegro
	assertInitialized(al_init(), "Allegro init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_install_mouse(), "Allegro mouse driver");
	assertInitialized(al_init_image_addon(), "Allegro image addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");
	assertInitialized(al_init_native_dialog_addon(), "Allegro dialong addon");

	// allegro display (antialiasing)
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST); // use multisampling
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}

void initializeVariables(ALLEGRO_TIMER*& timer, ALLEGRO_EVENT_QUEUE*& queue, ALLEGRO_DISPLAY*& display, ALLEGRO_FONT*& font)
{
	timer = al_create_timer(consts::frameTime);
	assertInitialized(timer, "timer");

	queue = al_create_event_queue();
	assertInitialized(queue, "event queue");

	display = al_create_display(consts::screenWidth, consts::screenHeight);
	assertInitialized(display, "display");

	font = al_create_builtin_font();
	assertInitialized(font, "font");
}

void createBalls(std::vector<Ball>& gameBalls, const int number, const double radius, const double mass)
{
	gameBalls.resize(number);
	for (int i{}; i < gameBalls.size(); ++i)
	{
		Ball& ball{ gameBalls[i] };
		ball.setRadius(radius);
		ball.setMass(mass);
		ball.setVisible(true);
		ball.setBallNumber(i);
	}
}

void moveToRackPositions(std::vector<Ball>& gameBalls)
{
	for (int i{}; i < gameBalls.size(); ++i)
	{
		gameBalls[i].setPosition(consts::rackBallPositions[i][0], consts::rackBallPositions[i][1]);
	}
}

void updatePhysics(std::vector<Ball>& gameBalls, const double updateDelta, std::vector<Player>& gamePlayers, TurnInformation& turnInfo)
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

		cueStick.setCuePower(0);
		// reset the cue stick position to make it seem like
		// it has been "shot"
		cueStick.updateAll(cueBall.getX(), cueBall.getY());
		cueStick.setCanUpdate(false);
	}
}

int main()
{
	initializeLibraries();

	ALLEGRO_TIMER* gameTimer; // this will act as the frame cap for the graphics
	ALLEGRO_DISPLAY* gameDisplay;
	ALLEGRO_FONT* gameFont;
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_EVENT currentEvent;

	initializeVariables(gameTimer, eventQueue, gameDisplay, gameFont);

	al_register_event_source(eventQueue, al_get_display_event_source(gameDisplay));
	al_register_event_source(eventQueue, al_get_timer_event_source(gameTimer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_mouse_event_source());

	al_set_window_title(gameDisplay, "Totally Accurate Billiards Simulator");

	Input& input{ Input::getInstance() };

	std::vector<Player> gamePlayers(2);
	std::vector<Ball> gameBalls;
	CueStick gameStick{ true, true };
	TurnInformation currentTurn;

	int currentPlayerIndex{ getRandomInteger(0, gamePlayers.size() - 1) };
	bool gameRunning{ true };
	bool drawFrame{ true };
	int shootStartTime{};

	std::cout << "Player (" << (currentPlayerIndex + 1) << ") is taking the break shot.\n";
	currentTurn.turnPlayerIndex = currentPlayerIndex;

	//al_show_native_message_box(
	//	gameDisplay,
	//	"Break Shot",
	//	"Player taking the break is: ",
	//	std::to_string(currentPlayerIndex + 1).c_str(),
	//	nullptr,
	//	NULL
	//);

	createBalls(gameBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);
	moveToRackPositions(gameBalls);

	al_start_timer(gameTimer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
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

			if (gameStick.canUpdate())
			{
				if (input.isMouseButtonDown(1))
				{
					shootCueBall(gameBalls[0], gameStick, input);
					shootStartTime = al_get_time();
				}
			}
			else if (al_get_time() - shootStartTime > 1)
			{
				gameStick.setVisible(false);
			}

			if (!physics::areBallsMoving(gameBalls) && !gameStick.isVisible()) //  turn finished
			{
				std::cout << "--TURN OVER--\n";

				std::cout << "First Hit Type: " << static_cast<int>(currentTurn.firstHitBallType) << '\n';
				std::cout << "Player Index: " << currentTurn.turnPlayerIndex << '\n';
				//std::cout << "Valid Turn: " << currentTurn.isTurnValid << '\n';
				std::cout << "Player Ball Type: " << static_cast<int>(gamePlayers[currentPlayerIndex].getTargetBallType()) << '\n';
				for (Ball* ball : currentTurn.pocketedBalls)
				{
					std::cout << ball->getBallNumber() << '\n';
				}

				const bool hasFouled = !referee::isTurnValid(gamePlayers[currentPlayerIndex], currentTurn);

				if (gamePlayers[currentPlayerIndex].getTargetBallType() != BallType::undetermined)
				{
					for (Ball* ball : currentTurn.pocketedBalls)
					{
						if (ball->getBallType() == gamePlayers[currentPlayerIndex].getTargetBallType())
							gamePlayers[currentPlayerIndex].addGameScore(1);
						else
							gamePlayers[(currentPlayerIndex + 1) % gamePlayers.size()].addGameScore(1);
					}
				}

				std::cout << "FOULED: " << hasFouled << '\n';

				// switch turns
				if (hasFouled || currentTurn.pocketedBalls.size() == 0)
				{
					currentPlayerIndex = (currentPlayerIndex + 1) % gamePlayers.size();
				}

				gameStick.setCanUpdate(true);
				gameStick.setVisible(true);

				if (!gameBalls[0].isVisible())
				{
					gameBalls[0].setVelocity(0, 0);
					gameBalls[0].setPosition(250, 250);
					gameBalls[0].setVisible(true);
				}

				currentTurn = {};
				currentTurn.turnPlayerIndex = currentPlayerIndex;

				std::cout << "SCORES: " << gamePlayers[0].getGameScore() << ' ' << gamePlayers[1].getGameScore() << '\n';
				std::cout << "Current Player: " << (currentPlayerIndex + 1) << '\n';
			}

			if (input.isKeyDown(ALLEGRO_KEY_ESCAPE))
				gameRunning = false;

			drawFrame = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			input.keyDownHook(currentEvent.keyboard.keycode);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			input.keyUpHook(currentEvent.keyboard.keycode);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		}

		if (gameRunning && drawFrame && al_is_event_queue_empty(eventQueue))
		{
			render::drawPlaysurface();
			render::drawPockets();
			render::drawBalls(gameBalls, gameFont);
			render::drawCueStick(gameStick);
			al_flip_display();
			drawFrame = false;
		}
	}

	// freeing all the resources
	al_destroy_event_queue(eventQueue);
	al_destroy_timer(gameTimer);
	al_destroy_display(gameDisplay);
	al_destroy_font(gameFont);

	return EXIT_SUCCESS;
}
