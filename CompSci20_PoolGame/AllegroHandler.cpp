#include "AllegroHandler.h"
#include "constants.h"
#include "common.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
//#include <allegro5/allegro_image.h>

#include <iostream>
#include <string_view>
#include <cstdlib>

AllegroHandler::AllegroHandler()
{
	// library initialization
	assertInitialized(al_init(), "Allegro library init");
	assertInitialized(al_install_keyboard(), "Allegro keyboard driver");
	assertInitialized(al_install_mouse(), "Allegro mouse driver");
	assertInitialized(al_install_audio(), "Allegro audio");
	assertInitialized(al_init_acodec_addon(), "Allegro audio codec addon");
	assertInitialized(al_init_primitives_addon(), "Allegro primitives addon");
	assertInitialized(al_init_native_dialog_addon(), "Allegro dialong addon");
	//assertInitialized(al_init_image_addon(), "Allegro image addon");

	// set up allegro display antialiasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST); // use multisampling
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	// create crucial variables
	m_timer = al_create_timer(consts::frameTime);
	m_eventQueue = al_create_event_queue();
	m_display = al_create_display(consts::screenWidth, consts::screenHeight);
	m_font = al_create_builtin_font();

	// check if the crucial variables are initialized
	assertInitialized(m_timer, "Allegro timer");
	assertInitialized(m_eventQueue, "Allegro event queue");
	assertInitialized(m_display, "Allegro display");
	assertInitialized(m_font, "Allegro builtin font");

	// register event sources
	al_register_event_source(m_eventQueue, al_get_display_event_source(m_display));
	al_register_event_source(m_eventQueue, al_get_timer_event_source(m_timer));
	al_register_event_source(m_eventQueue, al_get_keyboard_event_source());
	al_register_event_source(m_eventQueue, al_get_mouse_event_source());

	loadResources();
}

// loads resources (bitmaps or audio) that the game needs
// in this case it is just an audio file
void AllegroHandler::loadResources()
{
	// allow 16 concurrent audio samples to be played
	assertInitialized(al_reserve_samples(16), "Audio reserve samples (16)");

	// load ball clack audio sample
	ALLEGRO_SAMPLE* ballClackSample{ al_load_sample("ball_clack_short.wav") };
	assertInitialized(ballClackSample, "ball clack sound");
	m_loadedSoundSamples.push_back(ballClackSample);
}

// if the class is on the stack, then this should get called automatically
AllegroHandler::~AllegroHandler()
{
	destroyResources();

	if (destroyTimer())
		std::cout << "[CLEANUP] timer\n";
	if (destroyEventQueue())
		std::cout << "[CLEANUP] event queue\n";
	if (destroyDisplay())
		std::cout << "[CLEANUP] display\n";
	if (destroyFont())
		std::cout << "[CLEANUP] font\n";
}

void AllegroHandler::destroyResources()
{
	// unload all audio samples
	for (ALLEGRO_SAMPLE*& sample : m_loadedSoundSamples)
	{
		al_destroy_sample(sample);
	}

	m_loadedSoundSamples.clear();
}

// ret can take in a pointer or boolean
// nullptr is interpreted as false and anything else is true
void AllegroHandler::assertInitialized(bool resource, const std::string_view resourceName)
{
	if (!resource)
	{
		std::cout << "\n[Critical component of Allegro failed to initialize]\n";
		std::cout << "Component that failed: " << resourceName << '\n';

		pauseProgram("Press [ENTER] to close the program...");

		std::exit(EXIT_FAILURE);
	}
}

ALLEGRO_TIMER*& AllegroHandler::getTimer()
{
	return m_timer;
}

ALLEGRO_DISPLAY*& AllegroHandler::getDisplay()
{
	return m_display;
}

ALLEGRO_FONT*& AllegroHandler::getFont()
{
	return m_font;
}

ALLEGRO_EVENT_QUEUE*& AllegroHandler::getEventQueue()
{
	return m_eventQueue;
}

ALLEGRO_EVENT& AllegroHandler::getEvent()
{
	return m_event;
}

ALLEGRO_SAMPLE* const& AllegroHandler::getAudioSample(AudioSamples sample) const
{
	return m_loadedSoundSamples.at(static_cast<int>(sample));
}

bool AllegroHandler::destroyTimer()
{
	if (!m_timer)
		return false;

	al_destroy_timer(m_timer);
	m_timer = nullptr;
	return true;
}

bool AllegroHandler::destroyDisplay()
{
	if (!m_display)
		return false;

	al_destroy_display(m_display);
	m_display = nullptr;
	return true;
}

bool AllegroHandler::destroyFont()
{
	if (!m_font)
		return false;

	al_destroy_font(m_font);
	m_font = nullptr;
	return true;
}

bool AllegroHandler::destroyEventQueue()
{
	if (!m_eventQueue)
		return false;

	al_destroy_event_queue(m_eventQueue);
	m_eventQueue = nullptr;
	return true;
}

void AllegroHandler::startTimer()
{
	al_start_timer(m_timer);
}

bool AllegroHandler::isEventQueueEmpty()
{
	return al_is_event_queue_empty(m_eventQueue);
}
