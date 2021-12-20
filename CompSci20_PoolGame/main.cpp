#include "Ball.h"
#include "constants.h"
#include "utilities.h"
#include "physics.h"
#include "render.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

void initialize_libraries()
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

void initialize_variables(ALLEGRO_TIMER*& timer, ALLEGRO_EVENT_QUEUE*& queue, ALLEGRO_DISPLAY*& display, ALLEGRO_FONT*& font)
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

void create_balls(std::vector<Ball>& vecBalls, const int number, const double radius, const double mass)
{
	vecBalls.resize(number);
	for (Ball& ball : vecBalls)
	{
		ball.setRadius(radius);
		ball.setMass(mass);
	}
}

int main()
{
	initialize_libraries();

	ALLEGRO_TIMER* timer; // this will act as the frame cap for the graphics
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;

	initialize_variables(timer, eventQueue, display, font);

	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_mouse_event_source());

#ifdef TESTING_RELEASE
	al_set_window_title(display, "Totally Accurate Billiards Simulator | Testing Edition");
	al_show_native_message_box(
		display,
		"Totally Accurate Billiards Simulator | Testing Edition",
		"Thank you for testing Totally Accurate Billiards Simulator.",
		"\"The game is very realistic. The only things missing are the pockets for the balls\" - Nima Raika\n"
		"\"The balls should be more slippery.\" - Benjamin Jelica",
		nullptr,
		NULL
	);
#else
	al_set_window_title(display, "Totally Accurate Billiards Simulator");
#endif

	std::vector<Ball> activeBalls;

	create_balls(activeBalls, 16, consts::defaultBallRadius, consts::defaultBallMass);

	for (Ball& ball : activeBalls)
	{
		ball.setPosition(getRandomInteger(50, 950), getRandomInteger(50, 450));
	}

	//activeBalls[0].setPosition(250, 250);
	//activeBalls[1].setPosition(450, 250);
	//activeBalls[2].setPosition(550, 250);

	ALLEGRO_EVENT currentEvent;
	bool gameRunning{ true };
	bool drawFrame{ true };
	bool allBallsStopped{};

	ALLEGRO_MOUSE_STATE mouseState;
	bool isMouseDown{ false };

	double previousTime{ al_get_time() };
	double currentTime{};
	double frameTime{};
	double timeAccumulator{};

	al_start_timer(timer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:

			currentTime = al_get_time();
			frameTime = currentTime - previousTime;
			previousTime = currentTime;

			// max frame time
			if (frameTime > 0.25)
				frameTime = 0.25;

			timeAccumulator += frameTime;

			//#ifdef DEBUG
			std::cout << "[FRAME TIME] " << frameTime << '\n';
			//#endif // DEBUG

			while (timeAccumulator >= consts::physicsUpdateDelta)
			{
				physics::stepPhysics(activeBalls, allBallsStopped);
				timeAccumulator -= consts::physicsUpdateDelta;
			}

			al_get_mouse_state(&mouseState);

			if (allBallsStopped && mouseState.buttons & 1) // left click
			{
				isMouseDown = true;
			}
			else if (isMouseDown)
			{
				activeBalls[0].setVelocity((mouseState.x - activeBalls[0].getX()) / 7.5, (mouseState.y - activeBalls[0].getY()) / 7.5);
				isMouseDown = false;
			}

			drawFrame = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			activeBalls[0].setVelocity(60, 0);
			activeBalls[1].setVelocity(-60, 0);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		}

		if (gameRunning && drawFrame && al_is_event_queue_empty(eventQueue))
		{
			render::renderFrame(activeBalls, mouseState, isMouseDown, font, allBallsStopped);
			drawFrame = false;
		}
	}

	// freeing all the resources
	al_destroy_event_queue(eventQueue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_font(font);

	return EXIT_SUCCESS;
}
