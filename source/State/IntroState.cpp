#include "../stdafx.h"
#include "IntroState.h"

void IntroState::initEntities()
{
}

void IntroState::renderGUI(sf::RenderTarget& target)
{
}



IntroState::IntroState(StateData* state_data)
	:State(state_data)
{
}

IntroState::~IntroState()
{
}

void IntroState::updateEvents()
{
}

void IntroState::updateImGui()
{
	ImGui::Begin("Loading##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse /*| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/);
	ImGui::SliderInt("Loading", &load, 0, 100, "%d%%");
	ImGui::End();
}

void IntroState::update(const float& dtime)
{
	if (load != 100)
	{
		load++;
		Sleep(5);
	}
	if (load >= 100)
	{
		load = 0;
		StateManager::getInstance().endState();
		StateManager::getInstance().addState(new MainMenuState(stateData));
	}

}

void IntroState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window.get();
}
