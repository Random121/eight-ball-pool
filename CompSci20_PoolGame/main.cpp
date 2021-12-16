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

void physics_step(std::vector<Ball>& vecBalls)
{
	// movement for balls
	for (Ball& ball : vecBalls)
		ball.update();

	// vector of objects which have collided and will have velocities resolved later
	// pointers are used here to pass the object by reference not copy
	std::vector<std::pair<Ball*, Ball*>> collidedBalls;

	static int cycle{};

	// resolve the positions for the collision
	for (Ball& ball1 : vecBalls)
	{
		for (Ball& ball2 : vecBalls)
		{
			if (ball1.isOverlappingBall(ball2))
			{
				collidedBalls.push_back(std::make_pair(&ball1, &ball2));

				const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };
				
				// calculate how much each ball should move by to stop overlapping
				// add a tiny value (resolveCollisionMagicDistance) to the distance
				// to stop them from touching each other (which would still be considered)
				// an overlap and break the ball physics
				const double ballOverlap{ ((ball2.getRadius() - (ballDistance - ball1.getRadius())) / 2.0) + consts::resolveCollisionMagicDistance };

				const double moveDistanceX{ ballOverlap * (ball1.getX() - ball2.getX()) / ballDistance };
				const double moveDistanceY{ ballOverlap * (ball1.getY() - ball2.getY()) / ballDistance };

				// alternating subtraction and addition between the balls
				// moves them in different directions rather than just into
				// each other or in the same direction
				ball1.addPosition(moveDistanceX, moveDistanceY);
				ball2.addPosition(-1.0 * moveDistanceX, -1.0 * moveDistanceY);

				std::cout << cycle << " " << moveDistanceX << " collision position.\n";
				std::cout << "Ball1: " << ball1.getX() << ", " << ball1.getVX() << '\n';
				std::cout << "Ball2: " << ball2.getX() << ", " << ball2.getVX() << '\n'; 
				cycle++;
			}
		}
	}
	
	// resolve velocities
	for (std::pair<Ball*, Ball*>& ballPair : collidedBalls)
	{
		Ball& ball1{ *(ballPair.first) };
		Ball& ball2{ *(ballPair.second) };

		const double ballDistance{ calcPythagoreanHyp(ball1.getX() - ball2.getX(), ball1.getY() - ball2.getY()) };

		// normal vector (from center to center)
		double normalX{ (ball1.getX() - ball2.getX()) / ballDistance };
		double normalY{ (ball1.getY() - ball2.getY()) / ballDistance };

		// tangent vector (line at where the circumferences touch) (perpendicular to normal)
		double tangentX{ -normalY };
		double tangentY{ normalX };

		// velocity on tangent vector
		double velTangent1{ dotProduct(ball1.getVX(), tangentX, ball1.getVY(), tangentY) };
		double velTangent2{ dotProduct(ball2.getVX(), tangentX, ball2.getVY(), tangentY) };

		// velocity on normal vector
		double velNormal1{ dotProduct(ball1.getVX(), normalX, ball1.getVY(), normalY) };
		double velNormal2{ dotProduct(ball2.getVX(), normalX, ball2.getVY(), normalY) };

		static constexpr double massBall1{ 5 };
		static constexpr double massBall2{ 5 };

		// momentum 1d
		double momentum1{ (velNormal1 * (massBall1 - massBall2) + 2.0 * massBall2 * velNormal2) / (massBall1 + massBall2) };
		double momentum2{ (velNormal2 * (massBall2 - massBall1) + 2.0 * massBall1 * velNormal1) / (massBall1 + massBall2) };

		// set tangent velocities
		ball1.setVelocity(
			tangentX * velTangent1 + normalX * momentum1,
			tangentY * velTangent1 + normalY * momentum1
		);

		ball2.setVelocity(
			tangentX * velTangent2 + normalX * momentum2,
			tangentY * velTangent2 + normalY * momentum2
		);
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

	std::vector<Ball> activeBalls(2);

	for (Ball& ball : activeBalls)
	{
		ball.setRadius(consts::defaultBallRadius);
		// ball.setPosition(getRandomInteger(0, 640), getRandomInteger(0, 480));
	}

	activeBalls[0].setPosition(250, 250);
	activeBalls[1].setPosition(750, 250);

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
			physics_step(activeBalls);
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
