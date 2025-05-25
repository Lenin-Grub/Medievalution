#pragma once
#include "Math.h"

template <typename T1, typename T2>
float Math::convertToPercentage(T1 first, T2 percentage) 
{
    return static_cast<float>(first) * static_cast<float>(percentage) / 100.0f;
}

template <typename T1, typename T2>
sf::Vector2f Math::setCentre(T1 first, T2 second) 
{
    return sf::Vector2f((first / 2), (second / 2));
}

template <typename T>
T Math::toPercent(T value, T range_min, T range_max) 
{
    if (range_min == range_max)
        return static_cast<T>(0);

    float normalized = (static_cast<float>(value - range_min)) / (static_cast<float>(range_max - range_min));

    return static_cast<T>(normalized * 100);
}

template <typename T>
T Math::fromPercent(T percentage, T range_min, T range_max) 
{
    if (percentage < 0)   percentage = 0;
    if (percentage > 100) percentage = 100;

    return static_cast<T>(range_min + (range_max - range_min) * (static_cast<float>(percentage) / 100.0f));
}