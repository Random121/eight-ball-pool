#pragma once

#include "Vector2.h"

#include <allegro5/allegro5.h>

#include <vector>
#include <array>

class Input
{
private:
	Input();
	Input(const Input&) = delete;

	const int m_keySeen{ 1 };
	const int m_keyReleased{ 2 };

	ALLEGRO_MOUSE_STATE m_mouseState;
	std::array<char, ALLEGRO_KEY_MAX> m_keyStates;

public:
	static Input& getInstance();

	void updateAllStates();
	void clearAllStates();

	bool isKeyDown(int keyCode);
	void keyUpHook(int keyCode);
	void keyDownHook(int keyCode);
	void updateKeyState();

	ALLEGRO_MOUSE_STATE& getMouseState();
	void updateMouseState();

	bool isMouseButtonDown(int button);
	Vector2 getMouseVector();
	int getMouseX();
	int getMouseY();
};
