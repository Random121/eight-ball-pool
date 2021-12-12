#include <stdlib.h>
#include <iostream>
#include <string>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

// ret can take in a pointer of a boolean
// nullptr is interpreted as false and anything else is true
void check_init(bool ret, const std::string& event)
{
    if (ret)
        return;

    std::cout << "[INIT ERROR]: " << event << '\n';
    exit(EXIT_FAILURE);
}

int main()
{
    check_init(al_init(), "Allegro init");
    check_init(al_install_keyboard(), "keyboard driver");
    check_init(al_init_image_addon(), "image addon");
    check_init(al_init_primitives_addon(), "primites addon");

    ALLEGRO_TIMER* timer{ al_create_timer(1.0 / 30.0) };
    check_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue{ al_create_event_queue() };
    check_init(queue, "event queue");

    ALLEGRO_DISPLAY* display{ al_create_display(640, 480) };
    check_init(display, "display");

    ALLEGRO_FONT* font{ al_create_builtin_font() };
    check_init(font, "font");

    ALLEGRO_BITMAP* mysha{ al_load_bitmap("mysha.png") };
    check_init(mysha, "mysha bitmap");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool gameRunning{ true };
    bool drawFrame{ true };
    ALLEGRO_EVENT event;

    // frame timer fires an event per the time specified
    // this will act as the frame cap for the graphics
    al_start_timer(timer);

    while (gameRunning)
    {
        // waits for and retrieves an event from the queue
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            drawFrame = true;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            gameRunning = false;
            break;
        }

        if (drawFrame && gameRunning && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(mysha, 100, 100, 0);
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            
            al_flip_display();

            drawFrame = false;
        }
    }

    al_destroy_bitmap(mysha);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return EXIT_SUCCESS;
}