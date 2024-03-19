#pragma once
#include "State.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "../Localisation/Localisation.hpp"

class StateMachine;

namespace sf
{
	class RenderWindow;
}

class MenuState final : public State
{
public:
	MenuState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);



	void pause()									override;
	void resume()									override;

	void updateEvents()								override;
	void updateImGui()								override;
	void update(const float& dtime)					override;
	void draw(sf::RenderTarget* target = nullptr)	override;

	void setBackground();
	void initLocalisation();

private:
	sf::Texture background;
	sf::Sprite	sprite;
	sf::RectangleShape shape;
	std::ifstream localisation_file;
};
