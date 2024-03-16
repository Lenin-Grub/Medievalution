#include "../stdafx.h"
#include "MainMenuState.hpp"

MainMenuState::MainMenuState(StateData& state_data, StateMachine& state_manager)
    :State(state_data),
    state_manager(state_manager)
{

}

MainMenuState::~MainMenuState()
{
}

bool MainMenuState::init()
{
	return false;
}

void MainMenuState::updateEvents()
{
}

void MainMenuState::updateImGui()
{
	ImGui::Begin("Game Menu", nullptr, ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Back"))
	{
		state_manager.switchTo(States::State_Intro,true);
	}

	ImGui::End();
}

void MainMenuState::update(const float& dtime)
{
}

void MainMenuState::render(sf::RenderTarget* target)
{
    sf::RectangleShape sh;
    sh.setSize(sf::Vector2f(50, 50));
    sh.setPosition(sf::Vector2f(500, 500));
    sh.setFillColor(sf::Color::Red);
    target->draw(sh);
}
