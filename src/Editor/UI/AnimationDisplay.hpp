#pragma once
#include <UI/IDisplay.hpp>
#include <Animation/AnimationEditor/AnimationEditor.hpp>

class AnimationDisplay
	: public UI::IDisplay
{
public:
	AnimationDisplay(sf::RenderWindow& window);
	virtual ~AnimationDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:
	AnimationEditor   animation_editor;
};