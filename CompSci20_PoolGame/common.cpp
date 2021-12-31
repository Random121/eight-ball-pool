#define NOMINMAX

#include "common.h"

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

//size_t
