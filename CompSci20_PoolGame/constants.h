#pragma once

#include "utilities.h"

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

	inline constexpr double collisionFriction{ 0.9 };
	inline constexpr double rollingFriction{ 0.01 }; // bigger = more friction
	inline constexpr double stoppingVelocity{ 0.01 };
}

//#define DEBUG
//#define TESTING_RELEASE