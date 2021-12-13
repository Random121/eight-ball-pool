#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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

struct Bouncer
{
    double x, y;
    double dx, dy;
};

void physics_step(std::vector<Bouncer>& objs)
{
    for (int i{ 0 }; i < objs.size(); ++i)
    {
        Bouncer& b{ objs[i] };

        // advance bouncer
        b.x += b.dx;
        b.y += b.dy;

        // check for boundaries in x-axis
        if (b.x - 30 < 0)
        {
            b.x += (0 - (b.x - 30));
            b.dx *= -1;
        }
        else if (b.x + 30 > 640)
        {
            b.x -= (b.x + 30 - 640);
            b.dx *= -1;
        }

        // check for boundaries in y-axis
        if (b.y - 30 < 0)
        {
            b.y += (0 - (b.y - 30));
            b.dy *= -1;
        }
        else if (b.y + 30 > 480)
        {
            b.y -= (b.y + 30 - 480);
            b.dy *= -1;
        }
    }
}

int main()
{
    srand(time(0));

    // further randomizes numbers as first rand() execution is not really random
    for (int i = 1; i < 5; ++i)
        rand();

    init_allegro();
    init_display_settings();

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

    std::vector<Bouncer> objs(10);

    for (int i{ 0 }; i < objs.size(); ++i)
    {
        Bouncer& b{ objs[i] };
        b.x = rand() % 640;
        b.y = rand() % 480;
        b.dx = (rand() / static_cast<double>(RAND_MAX) - 0.5) * 80;
        b.dy = (rand() / static_cast<double>(RAND_MAX) - 0.5) * 80;
    }

    while (gameRunning)
    {
        // waits for and retrieves an event from the queue
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            physics_step(objs);
            drawFrame = true;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            gameRunning = false;
            break;
        }

        if (gameRunning && drawFrame && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            for (int i{ 0 }; i < objs.size(); ++i)
            {
                Bouncer& b{ objs[i] };
                al_draw_filled_circle(b.x, b.y, 30, al_map_rgb_f(1, 1, 1));
            }
            
            al_flip_display();

            drawFrame = false;
        }
    }

    // freeing all the resources
    al_destroy_bitmap(mysha);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return EXIT_SUCCESS;
}