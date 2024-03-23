#pragma once
#include "../stdafx.h"

class Math
{
public:
    // �������������� � ��������. 
    template <typename T1, typename T2>
    float convertToPercentage(T1 first, T2 percentage);

    // ������������ ������ ������������ �������� �������.
    template <typename T1, typename T2>
    sf::Vector2f setCentre(T1 first, T2 second);
};

#include "Math.inl"