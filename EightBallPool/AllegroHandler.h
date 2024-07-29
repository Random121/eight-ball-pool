#pragma once

#include "common.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

#include <string_view>
#include <vector>

// custom class for sharing, initialization,  and destruction of allegro resources
class AllegroHandler
{
private:
	ALLEGRO_TIMER* m_timer;
	ALLEGRO_DISPLAY* m_display;
	ALLEGRO_FONT* m_font;
	ALLEGRO_EVENT_QUEUE* m_eventQueue;
	ALLEGRO_EVENT m_event;

	std::vector<ALLEGRO_SAMPLE*> m_loadedSoundSamples;

	void assertInitialized(bool resource, const std::string_view resourceName);
	void loadResources();
	void destroyResources();

public:
	AllegroHandler();
	~AllegroHandler();

	ALLEGRO_TIMER*& getTimer();
	ALLEGRO_DISPLAY*& getDisplay();
	ALLEGRO_FONT*& getFont();
	ALLEGRO_EVENT_QUEUE*& getEventQueue();
	ALLEGRO_EVENT& getEvent();

	void createDisplay();

	bool destroyTimer();
	bool destroyDisplay();
	bool destroyFont();
	bool destroyEventQueue();

	ALLEGRO_SAMPLE* const& getAudioSample(AudioSamples sample) const;

	void startTimer();
	void stopTimer();
	bool isEventQueueEmpty();
};
