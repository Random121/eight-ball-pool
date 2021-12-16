#include <iostream>
#include <string>
#include <cstdlib>

// ret can take in a pointer or boolean
// nullptr is interpreted as false and anything else is true
void assertInitialized(bool ret, const std::string& event)
{
	if (ret)
		return;

	std::cout << "[INIT ERROR]: " << event << '\n';
	exit(EXIT_FAILURE);
}

int getRandomInteger(int min, int max)
{
	static constexpr double squishFraction{ 1.0 / (RAND_MAX + 1) }; // squishes the rand number between 0-1
	return min + static_cast<int>((max - min + 1) * (std::rand() * squishFraction));
}

// math behind dot product from here https://www.mathsisfun.com/algebra/vectors-dot-product.html
double dotProduct(double x1, double x2, double y1, double y2)
{
	return (x1 * x2) + (y1 * y2);
}