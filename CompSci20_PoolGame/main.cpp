#include "Ball.h"
#include "constants.h"
#include "utils.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

void initialize_libraries()
{
	// random
	std::srand(std::time(nullptr));
	const int TEMP{ std::rand() }; // for better randomization

	// allegro
	assertInitialized(al_init(), "Allegro init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_init_image_addon(), "Allegro image addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");

	// allegro display (mainly for antialiasing)
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

	std::vector<Ball> activeBalls(2);

	for (Ball& ball : activeBalls)
	{
		ball.setRadius(consts::defaultBallRadius);
		//ball.setPosition(getRandomInteger(0, 640), getRandomInteger(0, 480));
	}

	activeBalls[0].setPosition(100, 100);
	activeBalls[1].setPosition(500, 100);

	bool gameRunning{ true };
	bool drawFrame{ true };
	ALLEGRO_EVENT currentEvent;

	al_start_timer(timer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:
			for (Ball& ball : activeBalls)
			{
				ball.update();
				for (Ball& ball2 : activeBalls)
				{
					if (ball.isOverlappingBall(ball2))
					{
						std::cout << "OVERLAPPING!\n";
						continue;
					}
				}
			}
			drawFrame = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			//for (Ball& ball : activeBalls)
				//ball.setVelocity(getRandomInteger(1, 25), getRandomInteger(1, 25));
			activeBalls[0].setVelocity(10000, 0);
			activeBalls[1].setVelocity(-10000, 0);
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

			// draw balls
			for (Ball& ball : activeBalls)
			{
				const Ball::position2d_type& pos{ ball.getPosition2d() };

				//draw actual circle
				al_draw_filled_circle(
					std::get<0>(pos), 
					std::get<1>(pos), 
					ball.getRadius(),
					al_map_rgb_f(1, 1, 1)
				);

				//draw ball border
				al_draw_circle(
					std::get<0>(pos),
					std::get<1>(pos),
					ball.getRadius(),
					al_map_rgb(0, 0, 0),
					consts::ballBorderThickness
				);
			}

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