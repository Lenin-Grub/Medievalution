#pragma once
#include "Component.h"
#include "System.h"

class PositionComponent :
    public Component
{
public:
    PositionComponent() { std::cout << "component Position added" << std::endl; }
};

