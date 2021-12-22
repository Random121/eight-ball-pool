#include "Ball.h"
#include "constants.h"
#include "utilities.h"
#include "physics.h"
#include "render.h"
#include "Input.h"

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
	{ const int TEMP{ std::rand() }; } // for better randomization

	// allegro
	assertInitialized(al_init(), "Allegro init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_install_mouse(), "Allegro mouse driver");
	assertInitialized(al_init_image_addon(), "Allegro image addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");

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
	for (Ball& ball : gameBalls)
	{
		ball.setRadius(radius);
		ball.setMass(mass);
		ball.setVisible(true);
	}
}

void layoutRack(std::vector<Ball>& gameBalls)
{
	for (int i{}; i < gameBalls.size(); ++i)
	{
		gameBalls[i].setPosition(consts::rackBallPositions[i][0], consts::rackBallPositions[i][1]);
	}
}

void updatePhysics(std::vector<Ball>& gameBalls, const double updateDelta)
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
		physics::stepPhysics(gameBalls);
		timeAccumulator -= updateDelta;
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
	std::vector<Ball> gameBalls;

	createBalls(gameBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);
	layoutRack(gameBalls);

	bool gameRunning{ true };
	bool drawFrame{ true };
	bool noBallsMoving{};
	bool isMouseDown{};
	int cueStickPower{};

	al_start_timer(gameTimer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:
			updatePhysics(gameBalls, consts::physicsUpdateDelta);
			noBallsMoving = !physics::areBallsMoving(gameBalls);
			input.updateAllStates();

			if (input.isKeyDown(ALLEGRO_KEY_A))
				gameBalls[0].addPosition(-1, 0);
			if (input.isKeyDown(ALLEGRO_KEY_D))
				gameBalls[0].addPosition(1, 0);
			if (input.isKeyDown(ALLEGRO_KEY_W))
				gameBalls[0].addPosition(0, -1);
			if (input.isKeyDown(ALLEGRO_KEY_S))
				gameBalls[0].addPosition(0, 1);

			if (noBallsMoving && input.isMouseButtonDown(1)) // left click
			{
				isMouseDown = true;
			}
			else if (isMouseDown)
			{
				gameBalls[0].setVelocity(
					(input.getMouseX() - gameBalls[0].getX()) / 7.5,
					(input.getMouseY() - gameBalls[0].getY()) / 7.5
				);
				isMouseDown = false;
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
			if (noBallsMoving)
				render::drawCueStick(gameBalls[0], input.getMouseState(), cueStickPower);
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
