#pragma once

#include <cmath>
#include <string>

int getRandomInteger(int min, int max);
void assertInitialized(bool ret, const std::string& event);
double dotProduct(double x1, double x2, double y1, double y2);

// this used to be used for friction calculations
template <typename T>
int getSign(T num)
{
	return (num > 0) - (num < 0);
}

// calculates the length of hypoteneus using pythagorean formula
template <typename T>
T calcPythagoreanHyp(T x, T y)
{
	return std::sqrt((x * x) + (y * y));
}

struct Rectangle
{
	int xPos1{};
	int yPos1{};
	int xPos2{};
	int yPos2{};
};