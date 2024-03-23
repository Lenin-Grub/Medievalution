#pragma once
#include "../stdafx.h"

class Math
{
public:
    // Конвертировать в проценты. 
    template <typename T1, typename T2>
    float convertToPercentage(T1 first, T2 percentage);

    // Центрировать объект относительно заданной позиции.
    template <typename T1, typename T2>
    sf::Vector2f setCentre(T1 first, T2 second);
};

#include "Math.inl"