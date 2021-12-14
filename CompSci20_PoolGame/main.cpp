#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "utils.h"
#include "Ball.h"

void init_allegro()
{
    check_init(al_init(), "Allegro init");
    check_init(al_install_keyboard(), "keyboard driver");
    check_init(al_init_image_addon(), "image addon");
    check_init(al_init_primitives_addon(), "primitives addon");
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
    srand(time(0));
    for (int i = 1; i < 5; ++i) rand(); // better randomization

    init_allegro();
    init_display_settings();

    ALLEGRO_TIMER* timer{ al_create_timer(1.0 / 60.0) };
    check_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue{ al_create_event_queue() };
    check_init(queue, "event queue");

    ALLEGRO_DISPLAY* display{ al_create_display(640, 480) };
    check_init(display, "display");

    ALLEGRO_FONT* font{ al_create_builtin_font() };
    check_init(font, "font");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool gameRunning{ true };
    bool drawFrame{ true };
    ALLEGRO_EVENT event;

    // frame timer fires an event per the time specified
    // this will act as the frame cap for the graphics
    al_start_timer(timer);

    std::vector<Ball> objs(10);

    for (Ball& obj : objs)
    {
        obj.setPosition(rand() % 640, rand() % 480);
        obj.setVelocity((rand() / static_cast<double>(RAND_MAX) - 0.5) * 10, (rand() / static_cast<double>(RAND_MAX) - 0.5) * 10);
    }

    while (gameRunning)
    {
        // waits for and retrieves an event from the queue
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            for (Ball& obj : objs)
                obj.stepMovement();
            drawFrame = true;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            gameRunning = false;
            break;
        }

        if (gameRunning && drawFrame && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int count{};
            for (Ball& obj : objs)
            {
                const std::tuple<double, double>& pos{ obj.getPosition() };
                al_draw_filled_circle(std::get<0>(pos), std::get<1>(pos), 30, al_map_rgb_f(1, 1, 1));
                al_draw_text(font, al_map_rgb(0, 0, 0), std::get<0>(pos), std::get<1>(pos) - 5, ALLEGRO_ALIGN_CENTER, std::to_string(count).c_str());
                count++;
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