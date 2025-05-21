#pragma once
#include "../../Common/Common.h"
#include "../Components/Components.hpp"


class MyClass
{
public:
	MyClass() = default;
	virtual ~MyClass() = default;

	virtual void update(entt::registry& registry, float delta_time) = 0;

};