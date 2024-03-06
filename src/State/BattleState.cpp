#include "BattleState.hpp"


BattleState::BattleState(StateData* state_data)
	:State(state_data)
{
}

BattleState::~BattleState()
{
}


void BattleState::updateEvents() 
{
	glFrustum(-1, 1, -1, 1, 2, 8);

}

void BattleState::updateImGui()
{
	ImGui::Begin("BattleMenu##", nullptr, ImGuiWindowFlags_NoTitleBar 
										| ImGuiWindowFlags_NoCollapse 
										| ImGuiWindowFlags_NoMove 
										| ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button((char*)ICON_LOG_IN "Выход##toMainMenu"))
	{
		endState();
	}

	ImGui::End();
}


void BattleState::update(const float& dtime)
{
}


void BattleState::render(sf::RenderTarget* target)
{

	if (!target)
		target = this->window.get();

	glRotatef(i++,1,0,0);
	glTranslatef(0, 0, -3);

	window->pushGLStates();

	window->popGLStates();

}

void BattleState::updateObserver()
{
}