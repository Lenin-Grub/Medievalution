#include "../stdafx.h"
#include "StateMachine.hpp"
#include "LoadingState.hpp"


LoadingState::LoadingState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{ data, machine, window, replace }
{
	LOG_INFO("State Loading\t Init");
}

void LoadingState::pause()
{
	LOG_INFO("State Loading\t Pause");

}

void LoadingState::resume()
{
	LOG_INFO("State Loading\t Resume");
}

void LoadingState::updateEvents()
{
	if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		state_machine.lastState();
	}
}

void LoadingState::updateImGui()
{
}

void LoadingState::update(const float& dtime)
{

}

void LoadingState::draw(sf::RenderTarget* target)
{
	window.clear();

	ImGui::SFML::Render(window);

	window.display();
}
