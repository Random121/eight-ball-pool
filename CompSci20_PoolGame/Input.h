#pragma once

#include <allegro5/allegro5.h>

#include <vector>

class Input
{
private:
	Input();
	Input(const Input&) = delete;

	const int m_keySeen{ 1 };
	const int m_keyReleased{ 2 };

	ALLEGRO_MOUSE_STATE m_mouseState;
	std::vector<char> m_keyStates;

public:
	static Input& getInstance();

	void updateAllStates();

	bool isKeyDown(int keyCode);
	void keyUpHook(int keyCode);
	void keyDownHook(int keyCode);
	void updateKeyState();

	void updateMouseState();
	ALLEGRO_MOUSE_STATE& getMouseState();
	bool isMouseButtonDown(int button);
	int getMouseX();
	int getMouseY();
};

