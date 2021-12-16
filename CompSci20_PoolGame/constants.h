#pragma once

namespace consts
{
    inline constexpr int screenWidth{ 1000 };
    inline constexpr int screenHeight{ 500 };
    inline constexpr double frameTime{ 1.0 / 30.0 };

    inline constexpr int playSurfaceX{ 30 };
    inline constexpr int playSurfaceY{ 30 };

    inline constexpr int defaultBallRadius{ 13 };
    inline constexpr int ballBorderThickness{ 2 };

    inline constexpr double frictionCoefficient{ 0.01 };
    inline constexpr double stoppingVelocity{ 0.01 };
    inline constexpr double resolveCollisionMagicDistance{ 5 };
}
