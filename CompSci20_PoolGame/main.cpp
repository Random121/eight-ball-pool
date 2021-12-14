#include "utils.h"
#include "Ball.h"
#include "constants.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

void init_allegro()
{
	assertInitialized(al_init(), "Allegro init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_init_image_addon(), "Allegro image addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");
}

void init_display_settings()
{
	// display settings for antialiasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}

int main()
{
	srand(time(nullptr));
	rand(); rand(); // for better randomization

	init_allegro();
	init_display_settings();

	ALLEGRO_TIMER* timer{ al_create_timer(1.0 / 60.0) };
	assertInitialized(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue{ al_create_event_queue() };
	assertInitialized(queue, "event queue");

	ALLEGRO_DISPLAY* display{ al_create_display(consts::screenWidth, consts::screenHeight) };
	assertInitialized(display, "display");

	ALLEGRO_FONT* font{ al_create_builtin_font() };
	assertInitialized(font, "font");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool gameRunning{ true };
	bool drawFrame{ true };
	ALLEGRO_EVENT event;

	std::vector<Ball> objs(1);

	for (Ball& obj : objs)
	{
		obj.setPosition(rand() % 640, rand() % 480);
		obj.setVelocity((rand() / static_cast<double>(RAND_MAX) - 0.5) * 50, (rand() / static_cast<double>(RAND_MAX) - 0.5) * 50);
	}

	// this will act as the frame cap for the graphics
	al_start_timer(timer);

	while (gameRunning)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			for (Ball& obj : objs)
				obj.update();
			drawFrame = true;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		}

		if (gameRunning && drawFrame && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(181, 101, 29));

			al_draw_filled_rectangle(
                consts::playSurfaceX,
                consts::playSurfaceY,
                consts::screenWidth - consts::playSurfaceX,
                consts::screenWidth - consts::playSurfaceY,
                al_map_rgb(0, 110, 0)
            );

			for (Ball& obj : objs)
			{
				const std::tuple<double, double>& pos{ obj.getPosition() };
				al_draw_circle(
                   std::get<0>(pos),
                   std::get<1>(pos),
                   consts::ballRadius,
                   al_map_rgb(0, 0, 0),
                   consts::ballBorderThickness
                );
				al_draw_filled_circle(
                    std::get<0>(pos),
                    std::get<1>(pos),
                    consts::ballRadius,
                    al_map_rgb(255, 255, 255)
                );
			}

			al_flip_display();

			drawFrame = false;
		}
	}

	// freeing all the resources
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return EXIT_SUCCESS;
}
