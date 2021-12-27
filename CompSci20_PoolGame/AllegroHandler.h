#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include <string_view>

// custom class for sharing, initialization,  and destruction of allegro resources
class AllegroHandler
{
private:
	ALLEGRO_TIMER* m_timer;
	ALLEGRO_DISPLAY* m_display;
	ALLEGRO_FONT* m_font;
	ALLEGRO_EVENT_QUEUE* m_eventQueue;
	ALLEGRO_EVENT m_event;

	void assertInitialized(bool resource, const std::string_view resourceName);

public:
	AllegroHandler();
	~AllegroHandler();

	ALLEGRO_TIMER*& getTimer();
	ALLEGRO_DISPLAY*& getDisplay();
	ALLEGRO_FONT*& getFont();
	ALLEGRO_EVENT_QUEUE*& getEventQueue();
	ALLEGRO_EVENT& getEvent();

	void startTimer();
	bool isEventQueueEmpty();
};

