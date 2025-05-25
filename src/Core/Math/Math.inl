#pragma once

template<typename T1, typename T2>
float Math::convertToPercentage(T1 first, T2 percentage)
{
    float result = std::floor(static_cast<float>(first * (percentage / 100.0)));
    return result;
}

template<typename T1, typename T2>
sf::Vector2f Math::setCentre(T1 first, T2 second)
{
    return sf::Vector2f((first / 2), (second / 2));
}