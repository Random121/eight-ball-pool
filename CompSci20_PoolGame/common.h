#pragma once

#include "Ball.h"

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
std::string_view getBallTypeName(Ball::BallSuitType type);

// way to index audio samples from the
// resource vector
enum class AudioSamples
{
	ball_clack,
	ball_pocket,
	total_samples
};

struct TurnInformation
{
	Ball::BallSuitType firstHitBallType{};
	Ball::ballsPointer_type pocketedBalls;
	bool startWithBallInHand{};
	// nice and descriptive
	bool targetBallsSelectedThisTurn{};
	bool didFoulNoRail{}; // No Rail rule
};

struct Rectangle
{
	int xPos1{};
	int yPos1{};
	int xPos2{};
	int yPos2{};
};
