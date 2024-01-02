#include "../stdafx.h"
#include "GameState.h"

void GameState::initEntities()
{
	world_map.init();
	//TODO инкапсулировать в WorldMap
	world_map.shader.setParameter("width", (float)world_map.map_image.getSize().x);// 3072.0);
	world_map.shader.setParameter("height",(float)world_map.map_image.getSize().y);// 2048.0);
	world_map.shader.setUniform("select_color", sf::Glsl::Vec4(world_map.select_color));
}

void GameState::renderGUI(sf::RenderTarget& target)								
{
}

GameState::GameState(StateData* state_data)
	:State(state_data)
{
	camera.initView();
	initEntities();
	updateEvents();
}

GameState::~GameState()
{
}

void GameState::updateEvents()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		camera.zoom();
		if (Input::isMouseReleased(sf::Mouse::Left))
		{
			sf::Color color = world_map.getColor();

			LOG_INFO("Color {0},{1},{2}", color.r, color.g, color.b);
			LOG_INFO(world_map.findProvinceByColor(color));

			world_map.owner_color = color;
			world_map.select_color = color;

			world_map.shader.setParameter("owner_color", world_map.owner_color);
			world_map.shader.setParameter("select_color",world_map.select_color);
		}
	}
}

void GameState::updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::ShowDemoWindow();
	ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (ImGui::Button(u8"Выход##toMainMenu"))
	{
		endState();
	}
	ImGui::End();

	std::string str = world_map.getProvinceName();
	char* chr = const_cast<char*>(str.c_str());

	ImGui::SetNextWindowBgAlpha(0.55f);
	ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::TextColored(ImVec4(1, 1, 1, 0.5),u8"Это карта мира\n" "Используй WSAD для навигации\n" "Используй колесико для масштабирования\n" "Это окошко можно передвинуть ЛКМ");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1,1,0,1),"Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse Window Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
	else
		ImGui::Text("Mouse Position: <invalid>");
	ImGui::Text("Mouse View Position: (%.1f,%.1f)",core::mousePosView.x, core::mousePosView.y);
	ImGui::Text(Localisation::getInstance().getStringByKey("T_Province_name").c_str(), chr);
	ImGui::Text(Localisation::getInstance().getStringByKey("T_Province_id").c_str(), world_map.getProvinceID());
	ImGui::Separator();
	ImGui::SliderFloat(Localisation::getInstance().getStringByKey("T_transperecny").c_str(), &world_map.transperency, 0.0f, 1.0f);
	ImGui::End();
}

void GameState::update(const float& dtime)
{
	updateMousePositions();
	camera.updateView(dtime);
	world_map.update();
}

void GameState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(core::view);

//	рисуем динамечские объекты

	world_map.draw(*target,sf::RenderStates::Default);

//	рисуем статические объекты после установки дефолтной камеры (gui и тд)

	target->setView(this->window->getDefaultView());
	this->renderGUI(*target);

//	возвращаем камеру вида игры

	target->setView(core::view);

}

void GameState::updateObserver()
{
}