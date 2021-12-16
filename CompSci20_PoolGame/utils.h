#pragma once

#include <string>

// min and max are inclusive
int getRandomInteger(int min, int max);
void assertInitialized(bool ret, const std::string& event);

// this used to be useful, now its just here
template <typename T>
int getSign(T num)
{
	return (num > 0) - (num < 0);
}