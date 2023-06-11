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
	
	//to do
	//json j;
	std::ifstream i("translation/rus.json");
	i >> j;
	sp.loadFromFile("resources/Icons/rus.png");

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

	std::string str = j.find("T_new_game").value();
	if (ImGui::Button(str.c_str(), ImVec2(125, 0)))
	{
		StateManager::getInstance().addState(new GameState(stateData));
	}
	str = j.find("T_settings").value();
	if (ImGui::Button(str.c_str(), ImVec2(125, 0)))
	{
		StateManager::getInstance().addState(new SettingsState(stateData));
	}

	// to do пример того как можно сделать локализацию
	/*
		файл json слева ключ, справа перевод
		в классе который отвечает за загрузку при выборе языка загружаются разные файлы
		languages - файл в котором прописаны языки (eng, ru, fr)
		ru - русский язык
		eng - английский язык

		пример ключа и перевода
		const char T_KEY_exit = u8 "Выход"
		const char T_KEY_game = u8 "Игра"
		const char T_KEY_settings = u8 "Настройки"
	
	*/

	//const char exit[] = u8"Выход";

	//std::map<const char, std::string> text_key;

	str = j.find("T_exit").value();
	if (ImGui::Button(str.data(), ImVec2(125, 0)))
	{
		endState();
	}
	sf::Sprite s;
	s.setTexture(sp);
	ImGui::ImageButton(s, sf::Vector2f(120, 78));
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