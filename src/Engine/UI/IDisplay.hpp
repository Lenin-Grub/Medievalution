#pragma once

#include <vector>
#include <memory>
#include <Common/IconText.h>

namespace UI
{
	class IDisplay
	{
	public:
		virtual ~IDisplay() = default;
		virtual void draw() = 0;
		virtual void update(const float& delta_time) {}
	};

	struct DisplayHolder
	{
		std::vector<std::unique_ptr<IDisplay>> displays;
	};
}