#include "Ball.h"
#include "constants.h"
#include "utilities.h"
#include "physics.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <utility> // pair

void initialize_libraries()
{
	// random
	std::srand(std::time(nullptr));
	const int TEMP{ std::rand() }; // for better randomization

	// allegro
	assertInitialized(al_init(), "Allegro init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_install_mouse(), "Allegro mouse driver");
	assertInitialized(al_init_image_addon(), "Allegro image addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");

	// allegro display (for antialiasing)
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
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

int main()
{
	initialize_libraries();

	ALLEGRO_TIMER* timer; // this will act as the frame cap for the graphics
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;

	initialize_variables(timer, eventQueue, display, font);

	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_mouse_event_source());

	std::vector<Ball> activeBalls(2);

	for (Ball& ball : activeBalls)
	{
		ball.setRadius(consts::defaultBallRadius);
		ball.setMass(10.0);
		ball.setPosition(getRandomInteger(0, 640), getRandomInteger(0, 480));
	}

	activeBalls[0].setPosition(510.1, 250);
	activeBalls[1].setPosition(510, 250);
	activeBalls[0].setVelocity(5, 0);
	activeBalls[1].setVelocity(-5, 0);

	ALLEGRO_EVENT currentEvent;
	bool gameRunning{ true };
	bool drawFrame{ true };

	ALLEGRO_MOUSE_STATE mouseState;
	bool downBefore{ false };
	double mouseX{ -1 };
	double mouseY{};

	double prevTime{ al_get_time() };
	double nowTime{};
	double deltaTime{};

	al_start_timer(timer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:
			
			// delta time calculations
			nowTime = al_get_time();
			deltaTime = (nowTime - prevTime) / consts::physicsDeltaTime;
			prevTime = nowTime;

			physics::stepPhysics(activeBalls, deltaTime);
			al_get_mouse_state(&mouseState);
			if (mouseState.buttons & 1)
			{
				mouseX = mouseState.x;
				mouseY = mouseState.y;
				downBefore = true;
			}
			else if (downBefore)
			{
				activeBalls[0].setVelocity((mouseX - activeBalls[0].getX()) / 5, (mouseY - activeBalls[0].getY()) / 5);
				mouseX = -1;
				downBefore = false;
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
			al_clear_to_color(al_map_rgb(181, 101, 29));

			// draw play surface
			al_draw_filled_rectangle(
				consts::playSurfaceX,
				consts::playSurfaceY,
				consts::screenWidth - consts::playSurfaceX,
				consts::screenHeight - consts::playSurfaceY,
				al_map_rgb(0, 110, 0)
			);

			int counter{};
			for (Ball& ball : activeBalls)
			{
				//draw actual circle
				al_draw_filled_circle(
					ball.getX(), 
					ball.getY(),
					ball.getRadius(),
					al_map_rgb(255, (counter == 0) ? 255 : 0, 255)
				);

				//draw ball border
				al_draw_circle(
					ball.getX(),
					ball.getY(),
					ball.getRadius(),
					al_map_rgb(0, 0, 0),
					consts::ballBorderThickness
				);

				counter++;
			}

			if (mouseX != -1)
				al_draw_line(mouseX, mouseY, activeBalls[0].getX(), activeBalls[0].getY(), al_map_rgb(0, 0, 255), 3);

			al_flip_display();

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
