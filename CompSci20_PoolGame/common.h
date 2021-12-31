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

// min and max are inclusive
int getRandomInteger(const int min, const int max);
void pauseProgram(const std::string_view message);
void clearConsole(const char fillCharacter = ' ');
void resetCin();
void intArrayFisherYatesShuffle(std::vector<int>& intArray);

enum class BallType
{
	unknown,
	solid,
	striped,
	eight,
	cue
};

// way to index audio samples from the
// resource vector
enum class AudioSamples
{
	ball_clack,
	total_samples
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
	bool didFoulNoRail{}; // No Rail rule
};

struct Rectangle
{
	int xPos1{};
	int yPos1{};
	int xPos2{};
	int yPos2{};
};
