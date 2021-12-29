#pragma once

#include <string_view>
#include <cmath>
#include <vector>

// calculates the length of hypotenuse using pythagorean formula
template <typename T>
T calculateHypotenuse(T x, T y)
{
	return std::sqrt((x * x) + (y * y));
}

// this used to be used for friction calculations
// a snipper from stackoverflow
template <typename T>
int getSign(T num)
{
	return (num > 0) - (num < 0);
}

void pauseProgram(const std::string_view message);
// min and max are inclusive
int getRandomInteger(int min, int max);
void clearConsole(const char fillCharacter = ' ');
void resetCin();

enum class BallType
{
	unknown,
	solid,
	striped,
	eight,
	cue
};

// this forward declaration is needed rather than an include
// because Ball.h includes this file and will cause circular
// dependencies, doing this will only bring the class
class Ball;

struct TurnInformation
{
	BallType firstHitBallType{};
	std::vector<Ball*> pocketedBalls;
	bool startWithBallInHand{};
	// nice and descriptive
	bool isTargetBallsSelectedThisTurn{};
};

struct Rectangle
{
	int xPos1{};
	int yPos1{};
	int xPos2{};
	int yPos2{};
};