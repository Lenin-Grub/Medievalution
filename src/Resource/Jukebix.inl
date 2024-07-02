#pragma once
#include "Jukebox.hpp"

template <typename T>
void shuffle_range(T first, T last)
{
    std::random_device rd;
    auto g = std::mt19937(rd());
    std::shuffle(first, last, g);
}