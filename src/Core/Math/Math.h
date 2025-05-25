#pragma once

class Math 
{
public:
    /// @brief Converts a value to a percentage of another value.
    template <typename T1, typename T2>
    static float convertToPercentage(T1 first, T2 percentage);

    /// @brief Center an object relative to a given position.
    template <typename T1, typename T2>
    static sf::Vector2f setCentre(T1 first, T2 second);

    /// @brief Maps a value from its range to a percentage [0..100].
    template <typename T>
    static T toPercent(T value, T range_min, T range_max);

    /// @brief Maps a percentage [0..100] back to a value in the given range.
    template <typename T>
    static T fromPercent(T percentage, T range_min, T range_max);
};

#include "Math.inl"