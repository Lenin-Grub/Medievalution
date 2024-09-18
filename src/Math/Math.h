#pragma once
#include "../stdafx.h"

class Math
{
public:
    /// @brief Converts a value to a percentage of another value.
    /// @tparam T1 Type of the first value.
    /// @tparam T2 Type of the percentage value.
    /// @param first The base value.
    /// @param percentage The percentage value.
    /// @return float The calculated percentage.
    template <typename T1, typename T2>
    float convertToPercentage(T1 first, T2 percentage);

    /// @brief Center an object relative to a given position.
    /// @tparam T1 Type of the first point.
    /// @tparam T2 Type of the second point.
    /// @param first The first point.
    /// @param second The second point.
    /// @return sf::Vector2f The centre point.
    template <typename T1, typename T2>
    sf::Vector2f setCentre(T1 first, T2 second);
};

#include "Math.inl"