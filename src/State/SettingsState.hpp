#pragma once
#include "State.hpp"
#include "../Localisation/Localisation.hpp"

class StateMachine;

class SettingsState final : public State
{
public:
	SettingsState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause()									override;
	void resume()									override;

	void updateEvents()								override;
	void updateImGui()								override;
	void update(const float& dtime)					override;
	void draw(sf::RenderTarget* target = nullptr)	override;

private:
	bool play_music;
	bool play_sound;
	sf::Texture background;
	sf::RectangleShape shape;

	std::vector<sf::VideoMode> video_modes;
	int resolution_current_id;
	std::string string_resolution;
};
