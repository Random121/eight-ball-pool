#include "Ball.h"
#include "constants.h"
#include "utilities.h"

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

void physics_step(std::vector<Ball>& vecBalls)
{
	// movement for balls
	for (Ball& ball : vecBalls)
		ball.update();

	// vector of objects which have collided and will have velocities resolved later
	// pointers are used here to pass the object by reference not copy
	std::vector<std::pair<Ball*, Ball*>> collidedBallPairs;

	static int cycle{};

	// resolve the positions for the collision
	for (int i{ 0 }; i < vecBalls.size() + 1; ++i)
	{
		for (int j{ i + 1 }; j < vecBalls.size(); ++j)
		{
			Ball& ball1{ vecBalls[i] };
			Ball& ball2{ vecBalls[j] };
			if (ball1.isOverlappingBall(ball2))
			{
				collidedBallPairs.push_back(std::make_pair(&ball1, &ball2));

				const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };
				
				// calculate how much each ball should move by to stop overlapping
				const double ballOverlap{ ((ballDistance - ball1.getRadius() - ball2.getRadius()) / 2.0) };

				const double moveDistanceX{ ballOverlap * (ball1.getX() - ball2.getX()) / ballDistance };
				const double moveDistanceY{ ballOverlap * (ball1.getY() - ball2.getY()) / ballDistance };

#ifdef DEBUG
				std::cout << "[POSITION RESOLUTION]\n";
				std::cout << "[distance] " << ballDistance << '\n';
				std::cout << "[overlap] " << ballOverlap << '\n';
				std::cout << "[moveDistanceX] " << moveDistanceX << '\n';
				std::cout << "[moveDistanceY] " << moveDistanceY << '\n';
				std::cout << "[start ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
				std::cout << "[start ball2] " << ball2.getX() << ", " << ball2.getY() << '\n';
#endif // DEBUG


				// alternating subtraction and addition between the balls
				// moves them in different directions rather than just into
				// each other or in the same direction
				ball1.addPosition(-moveDistanceX, -moveDistanceY);
				ball2.addPosition(moveDistanceX, moveDistanceY);

#ifdef DEBUG
				std::cout << "[end ball1] " << ball1.getX() << ", " << ball1.getY() << '\n';
				std::cout << "[end ball2] " << ball2.getX() << ", " << ball2.getY() << "\n\n";
#endif // DEBUG

			}
		}
	}
	
	// resolve velocities
	for (std::pair<Ball*, Ball*>& ballPair : collidedBallPairs)
	{
		Ball& ball1{ *(ballPair.first) };
		Ball& ball2{ *(ballPair.second) };

		const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };

		// normal vector (from center to center)
		const double normalX{ (ball1.getX() - ball2.getX()) / ballDistance };
		const double normalY{ (ball1.getY() - ball2.getY()) / ballDistance };

		const double deltaVelocityX{ ball1.getVX() - ball2.getVX() };
		const double deltaVelocityY{ ball1.getVY() - ball2.getVY() };

		const double p{ 2.0 * dotProduct(normalX, deltaVelocityX, normalY, deltaVelocityY) / (ball1.getMass() + ball2.getMass()) };
		const double newVelocityX{ p * normalX };
		const double newVelocityY{ p * normalY };

#ifdef DEBUG
		std::cout << "[START VELOCITY] " << ball1.getVX() << ", " << ball2.getVX() << 
#endif // DEBUG


		ball1.addVelocity(
			-(newVelocityX * ball2.getMass()),
			-(newVelocityY * ball2.getMass())
		);

		ball2.addVelocity(
			(newVelocityX * ball1.getMass()),
			(newVelocityY * ball1.getMass())
		);

#ifdef DEBUG
		std::cout << "[END VELOCITY] " << ball1.getVX() << ", " << ball2.getVX() << ", " << ballDistance << "\n\n";
#endif // DEBUG

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

	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_mouse_event_source());

	std::vector<Ball> activeBalls(6);

	for (Ball& ball : activeBalls)
	{
		ball.setRadius(consts::defaultBallRadius);
		ball.setMass(100.0);
		ball.setPosition(getRandomInteger(0, 640), getRandomInteger(0, 480));
	}

	activeBalls[0].setPosition(250, 250);
	activeBalls[1].setPosition(750, 250);

	bool gameRunning{ true };
	bool drawFrame{ true };
	ALLEGRO_EVENT currentEvent;

	ALLEGRO_MOUSE_STATE mouseState;
	bool downBefore{ false };
	double mouseX{ -1337 };
	double mouseY{};

	al_start_timer(timer);

	while (gameRunning)
	{
		al_wait_for_event(eventQueue, &currentEvent);

		switch (currentEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:
			physics_step(activeBalls);
			//al_mouse_button_down(&mouseState, 1);
			al_get_mouse_state(&mouseState);
			if (mouseState.buttons & 1)
			{
				mouseX = mouseState.x;
				mouseY = mouseState.y;
				downBefore = true;
			}
			else if (downBefore)
			{
				activeBalls[0].setVelocity((activeBalls[0].getX() - mouseX) / -5, (activeBalls[0].getY() - mouseY) / -5);
				mouseX = -1337;
				downBefore = false;
			}
			drawFrame = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			// for (Ball& ball : activeBalls)
				// ball.setVelocity(getRandomInteger(-25, 25), getRandomInteger(-25, 25));

			activeBalls[0].setVelocity(50, 0);
			activeBalls[1].setVelocity(-50, 0);
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
			// draw balls
			for (Ball& ball : activeBalls)
			{
#ifdef DEBUG
				std::cout << "[DRAWING] " << ball.getX() << ", " << ball.getY() << ", " << ball.getRadius() << "\n\n";
#endif // DEBUG


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

			if (mouseX != -1337)
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
