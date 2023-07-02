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
	language_rus.loadFromFile("resources/Flags/rus.png");
	language_eng.loadFromFile("resources/Flags/eng.png");
	
	if (stateData->graphicSettings->language == "rus")
	{
		i.open("translation/rus.json");
		i >> stateData->graphicSettings->j;
		sprite.setTexture(language_rus);

	}
	if (stateData->graphicSettings->language == "eng")
	{
		i.open("translation/eng.json");
		i >> stateData->graphicSettings->j;
		sprite.setTexture(language_eng);
	}
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

	std::string str = stateData->graphicSettings->j.at("T_new_game");
	if (ImGui::Button(str.c_str(), ImVec2(120, 0)))
	{
		StateManager::getInstance().addState(new GameState(stateData));
	}
	str = stateData->graphicSettings->j.at("T_settings");
	if (ImGui::Button(str.c_str(), ImVec2(120, 0)))
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

	str = stateData->graphicSettings->j.at("T_exit");
	if (ImGui::Button(str.data(), ImVec2(120, 0)))
	{
		endState();
	}

	static int counter = 0;
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x + 64;
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) 
	{
		sprite.setTexture(language_rus);
		stateData->graphicSettings->language = "rus";
		stateData->graphicSettings->saveToFile("config/graphic_settings.ini");

	}

	ImGui::SameLine(0.0f, spacing);

	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		sprite.setTexture(language_eng);
		stateData->graphicSettings->language = "eng";
		stateData->graphicSettings->saveToFile("config/graphic_settings.ini");
	}
	ImGui::Image(sprite, sf::Vector2f(120, 78));
	ImGui::End();



	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
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

}
//------------------------------------------------------------------------------------------------------------------------