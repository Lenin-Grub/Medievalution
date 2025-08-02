#pragma once
#include <UI/IDisplay.hpp>

class SceneHierrarhyDisplay
	: public UI::IDisplay
{
public:
	SceneHierrarhyDisplay() = default;
	virtual ~SceneHierrarhyDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:

};