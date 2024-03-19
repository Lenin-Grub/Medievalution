#pragma once
#include "State.hpp"
#include "../Math/Math.h"
#include "../Input/Input.h"

class StateMachine;

class IntroState final : public State
{
public:
	IntroState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause()									override;
	void resume()									override;

	void updateEvents()								override;
	void updateImGui()								override;
	void update(const float& dtime)					override;
	void draw(sf::RenderTarget* target = nullptr)	override;

	void setBackground();

private:
	sf::Texture background;
	sf::Sprite	sprite;
	sf::RectangleShape shape;
	sf::Color alpha;
	sf::Text text;
	sf::Font font;
};
