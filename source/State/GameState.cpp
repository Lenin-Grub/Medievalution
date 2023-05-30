﻿#include "../stdafx.h"
#include "GameState.h"

//------------------------------------------------------------------------------------------------------------------------
// инициализация сущностей
void GameState::initEntities()
{
	_map.init();
}


//------------------------------------------------------------------------------------------------------------------------
//рисуем кнопки
void GameState::renderGUI(sf::RenderTarget& target)								
{
}

//------------------------------------------------------------------------------------------------------------------------
// конструктор
GameState::GameState(StateData* state_data)
	:State(state_data)
{
	camera.initView();
	initEntities();				// сущности
	updateEvents();				// разовый вызов обновления событий для корректного отображения кнопок
}

//------------------------------------------------------------------------------------------------------------------------
// деструктор
GameState::~GameState()
{
	
}

//------------------------------------------------------------------------------------------------------------------------
// обвновление событий
void GameState::updateEvents()
{
	camera.scroll();
}

//------------------------------------------------------------------------------------------------------------------------
// обновление IMGUI
void GameState::updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::ShowDemoWindow();
	ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (ImGui::Button("Exit##toMainMenu"))
	{
		endState();
	}
	ImGui::End();

	std::string str = _map.getProvinceName();
	char* chr = const_cast<char*>(str.c_str());

	ImGui::SetNextWindowBgAlpha(0.55f); // Transparent background
	ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::TextColored(ImVec4(1, 1, 1, 0.5),"This is world map\n" "Use WSAD to move\n" "Use mouse wheel to scroll and zooming");
	ImGui::Separator();
	//ImGui::Text("FPS: %d", io.Framerate);
	ImGui::TextColored(ImVec4(1,1,0,1),"Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse Window Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
	else
		ImGui::Text("Mouse Position: <invalid>");
	ImGui::Text("Mouse View Position: (%.1f,%.1f)",core::mousePosView.x, core::mousePosView.y);
	ImGui::Text("Province name: %s" , chr);
	ImGui::Text("Province id: %d", _map.getProvinceID());
	ImGui::Separator();
	ImGui::SliderFloat("transperency", &_map.transperency,0.0f,1.0f);
	ImGui::End();
}

//------------------------------------------------------------------------------------------------------------------------
// обновление всей логики
void GameState::update(const float& dtime)
{
	updateMousePositions();
	camera.updateView(dtime);
	_map.update();
}

//------------------------------------------------------------------------------------------------------------------------
//рисуем все
void GameState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(core::view);
//------------------------------------------------------------------------------------------------------------------------
//	рисуем динамечсеие объекты
//------------------------------------------------------------------------------------------------------------------------
	_map.draw(*target,sf::RenderStates::Default);
//------------------------------------------------------------------------------------------------------------------------
//	рисуем статические объекты после установки дефолтной камеры (gui и тд)
//------------------------------------------------------------------------------------------------------------------------
	target->setView(this->window->getDefaultView());
	this->renderGUI(*target);
//------------------------------------------------------------------------------------------------------------------------
//	возвращаем камеру вида игры
//------------------------------------------------------------------------------------------------------------------------
	target->setView(core::view);

}
//------------------------------------------------------------------------------------------------------------------------
