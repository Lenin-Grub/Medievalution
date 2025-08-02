#pragma once
#include <UI/IDisplay.hpp>

class LogDisplay
	: public UI::IDisplay
{
public:
	LogDisplay() = default;
	virtual ~LogDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:

};