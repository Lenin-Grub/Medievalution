#include "../stdafx.h"
#include "IntroState.hpp"

IntroState::IntroState(StateData& state_data, StateMachine& state_manager)
    :State(state_data),
    state_manager(state_manager)
{

}

IntroState::~IntroState()
{
}

bool IntroState::init()
{
	return false;
}

void IntroState::updateEvents()
{
}

void IntroState::updateImGui()
{
	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("New Game"))
	{
		state_manager.switchTo(States::State_Loading);
	}

	ImGui::End();
}

void IntroState::update(const float& dtime)
{
}

void IntroState::render(sf::RenderTarget* target)
{
    sf::RectangleShape sh;
    sh.setSize(sf::Vector2f(50, 50));
    sh.setPosition(sf::Vector2f(50, 50));
    target->draw(sh);
}
