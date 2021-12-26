#include "Input.h"

#include "Vector2.h"

#include <allegro5/allegro5.h>

Input::Input()
{
}

Input& Input::getInstance()
{
	static Input* instance;
	if (instance == nullptr)
	{
		instance = new Input();
		instance->m_keyStates.resize(ALLEGRO_KEY_MAX);
	}
	return *instance;
}

bool Input::isKeyDown(int keyCode)
{
	return m_keyStates.at(keyCode);
}

void Input::keyUpHook(int keyCode)
{
	m_keyStates[keyCode] &= m_keyReleased;
}

void Input::keyDownHook(int keyCode)
{
	m_keyStates[keyCode] = m_keySeen | m_keyReleased;
}

void Input::updateKeyState()
{
	for (int i{}; i < ALLEGRO_KEY_MAX; ++i)
	{
		m_keyStates[i] &= m_keySeen;
	}
}

void Input::updateMouseState()
{
	al_get_mouse_state(&m_mouseState);
}

bool Input::isMouseButtonDown(int button)
{
	return (m_mouseState.buttons & button);
}

ALLEGRO_MOUSE_STATE& Input::getMouseState()
{
	return m_mouseState;
}

int Input::getMouseX()
{
	return m_mouseState.x;
}

int Input::getMouseY()
{
	return m_mouseState.y;
}

Vector2 Input::getMouseVector()
{
	return Vector2(m_mouseState.x, m_mouseState.y);
}

void Input::updateAllStates()
{
	updateKeyState();
	updateMouseState();
}