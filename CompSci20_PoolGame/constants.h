#pragma once

namespace consts
{
    inline constexpr int screenWidth{ 1000 };
    inline constexpr int screenHeight{ 500 };
    inline constexpr double frameTime{ 1.0 / 75.0 };
    inline constexpr double physicsDeltaTime{ 1 / 60.0 };

    inline constexpr int playSurfaceX{ 30 };
    inline constexpr int playSurfaceY{ 30 };

    inline constexpr int defaultBallRadius{ 15 };
    inline constexpr int ballBorderThickness{ 2 };

    inline constexpr double rollingFriction{ 0.98 };
    inline constexpr double collisionFriction{ 0.98 };
    inline constexpr double stoppingVelocity{ 0.05 };
}

#define DEBUG
#define PHYSICS_BALL_VELOCITY_CAP