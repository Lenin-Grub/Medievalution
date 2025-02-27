#pragma once
#include "../../Common/Common.h"
#include "../Components/Components.hpp"

class ISystem
{
public:
	ISystem()            = default;
	virtual ~ISystem()   = default;

	virtual void update() = 0;

private:

};
