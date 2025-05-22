#pragma once
#include "../../Common/Common.h"
#include "../Components/Components.hpp"


class ISystem
{
public:
	ISystem() = default;
	virtual ~ISystem() = default;

	virtual void update(entt::registry& registry, float delta_time) = 0;

};