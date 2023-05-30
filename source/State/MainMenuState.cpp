//------------------------------------------------------------------------------------------------------------------------
#include "../stdafx.h"
#include "MainMenuState.h"

//------------------------------------------------------------------------------------------------------------------------
MainMenuState::MainMenuState(StateData* state_data)
	:State(state_data)
{
	this->updateEvents();				// разовый вызов обновления событий для корректного отображения кнопок
	core::music->setVolume(stateData->graphicSettings->music_volume);
	LOG_INFO("Main menu constructor not finished. Count of state {}", StateManager::getInstance().states.size());
}

//------------------------------------------------------------------------------------------------------------------------
MainMenuState::~MainMenuState(){}

//------------------------------------------------------------------------------------------------------------------------
void MainMenuState::updateEvents()
{
}

//------------------------------------------------------------------------------------------------------------------------
void MainMenuState::updateImGui()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Main Menu",nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (ImGui::Button("Game", ImVec2(120, 0)))
	{
		StateManager::getInstance().addState(new GameState(stateData));
	}

	if (ImGui::Button("Settings", ImVec2(120, 0)))
	{
		StateManager::getInstance().addState(new SettingsState(stateData));
	}

	if (ImGui::Button("Exit", ImVec2(120, 0)))
	{
		endState();
	}
	ImGui::End();


	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

//------------------------------------------------------------------------------------------------------------------------
//обновляем все
void MainMenuState::update(const float& dtime)			
{
	this->updateMousePositions();
}

//------------------------------------------------------------------------------------------------------------------------
//рисуем все
void MainMenuState::render(sf::RenderTarget* target)							
{ 
	if (!target)
		target = this->window.get();
	target->setView(this->window->getDefaultView());
	target->draw(this->mouseCordsText);											// координаты
}
//------------------------------------------------------------------------------------------------------------------------