#include "scheduler.h"
#include "constants.h"
#include "physics.h"

#include <allegro5/allegro5.h>


namespace scheduler
{
	double previousTime{ al_get_time() };
	double currentTime{};
	double frameTime{};
	double timeAccumulator{};

	void physicsUpdate()
	{
		currentTime = al_get_time();
		frameTime = currentTime - previousTime;
		previousTime = currentTime;

		// max frame time
		if (frameTime > 0.25)
			frameTime = 0.25;

		timeAccumulator += frameTime;

		while (timeAccumulator >= consts::physicsUpdateDelta)
		{
			//physics::stepPhysics(activeBalls);
			timeAccumulator -= consts::physicsUpdateDelta;
		}
	}

	// probably not the best way to do this
	void handleEvents(ALLEGRO_EVENT activeEvent, bool& gameRunning)
	{
		switch (activeEvent.type)
		{
		case ALLEGRO_EVENT_TIMER:
			physicsUpdate();

			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			gameRunning = false;
			break;
		default:
			break;
		}
	}
}