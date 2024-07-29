#include "common.h"

#include "Ball.h"

// for some windows api optimizations
#define WIN32_LEAN_AND_MEAN

// required as windows min max interferes with the one from numeric_limits
#define NOMINMAX

#include <Windows.h>

#include <iostream>
#include <string_view>
#include <cstdlib>
#include <limits>

int getRandomInteger(const int min, const int max)
{
	return std::rand() % (max - min + 1) + min;
}

// ***snippet from stackoverflow***
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

void pauseProgram(const std::string_view message)
{
	std::cout << message;
	std::cin.get();
}

void resetCin()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// my own c++ version of the Fisher-Yates shuffle pseudocode from wikipedia

void intArrayFisherYatesShuffle(std::vector<int>& intArray)
{
	int randIndex;
	int tempVal;
	for (int i{ static_cast<int>(intArray.size()) - 1 }; i > 0; --i)
	{
		randIndex = getRandomInteger(0, i);

		// swap elements
		tempVal = intArray[i];
		intArray[i] = intArray[randIndex];
		intArray[randIndex] = tempVal;
	}
}

std::string_view getBallTypeName(Ball::BallSuitType type)
{
	switch (type)
	{
	case Ball::BallSuitType::unknown:
		return "Unknown";
	case Ball::BallSuitType::solid:
		return "Solid";
	case Ball::BallSuitType::striped:
		return "Striped";
	case Ball::BallSuitType::eight:
		return "Eight-Ball";
	case Ball::BallSuitType::cue:
		return "Cue-Ball";
	default:
		return "???";
	}
}
