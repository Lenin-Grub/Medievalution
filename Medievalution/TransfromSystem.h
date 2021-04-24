#pragma once
#include "SystemBase.h"
#include "System.h"

class TransfromSystem :
    public SystemBase
{
public:
    TransfromSystem() { std::cout << "transform system add!" << std::endl; }

    void updateSystem() override { std::cout << "transform system work!" << std::endl; };
};

