#define NOMINMAX

#include "common.h"

#include <Windows.h>

#include <iostream>
#include <string_view>
#include <cstdlib>
#include <limits>


// min and max are inclusive
int getRandomInteger(int min, int max)
{
	static constexpr double squishFraction{ 1.0 / (RAND_MAX + 1) }; // squishes the rand number between 0-1
	return min + static_cast<int>((max - min + 1) * (std::rand() * squishFraction));
}

void pauseProgram(const std::string_view message)
{
	std::cout << message;
	std::cin.get();
}

void clearConsole(const char fillCharacter)
{
	COORD topLeft{ 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) };
	GetConsoleScreenBufferInfo(hConsole, &info);
	DWORD written;
	int cells{ info.dwSize.X * info.dwSize.Y };
	FillConsoleOutputCharacter(hConsole, fillCharacter, cells, topLeft, &written);
	FillConsoleOutputAttribute(hConsole, info.wAttributes, cells, topLeft, &written);
	SetConsoleCursorPosition(hConsole, topLeft);
}

void resetCin()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}