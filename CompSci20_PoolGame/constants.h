#pragma once

#include "utilities.h"
#include <vector>

namespace consts
{
	inline constexpr int screenWidth{ 1000 };
	inline constexpr int screenHeight{ 500 };

	inline constexpr int playSurfaceX{ 40 };
	inline constexpr int playSurfaceY{ 40 };

	inline constexpr Rectangle playSurface{
		consts::playSurfaceX,
		consts::playSurfaceY,
		consts::screenWidth - consts::playSurfaceX,
		consts::screenHeight - consts::playSurfaceY
	};

	inline constexpr double frameTime{ 1.0 / 120.0 };
	inline constexpr double physicsUpdateDelta{ 1.0 / 60.0 };

	inline constexpr int defaultBallRadius{ 15 };
	inline constexpr int defaultBallMass{ 10 };
	inline constexpr int ballBorderThickness{ 2 };

	inline constexpr double collisionFriction{ 0.9 }; // smaller = more friction
	inline constexpr double rollingFriction{ 0.011 }; // bigger = more friction
	inline constexpr double stoppingVelocity{ 0.01 };

	inline constexpr double cueStickDistanceFromBall{ 17.0 };
	inline constexpr double cueStickLength{ 400.0 };
	inline constexpr double cueStickThickness{ 7 };
	inline constexpr double cueStickWoodThickness{ cueStickThickness - 5 };

	inline constexpr double pocketRadius{ 23 };
	inline const std::vector<std::vector<int>> pocketCoordinates{
		{45, 45},
		{500, 30},
		{955, 45},
		{45, 455},
		{500, 470},
		{955, 465}
	};

	inline const std::vector<std::vector<int>> ballColorMap{
		{200, 200, 0}, // yellow
		{10, 20, 214}, // blue
		{247, 13, 26}, // light red
		{191, 0, 255}, // purple
		{255, 94, 14}, // orange
		{30, 150, 30}, // green
		{150, 0, 0}, // dark red
		{0, 0, 0} // black
	};

	inline const std::vector<std::vector<int>> rackBallPositions{
		{250, 250},
		{802, 212},
		{857, 275},
		{829, 227},
		{856, 210},
		{856, 180},
		{829, 290},
		{774, 259},
		{803, 244},
		{747, 243},
		{830, 259},
		{857, 306},
		{775, 228},
		{856, 241},
		{829, 195},
		{802, 275}
	};
}

//#define DEBUG
//#define TESTING_RELEASE