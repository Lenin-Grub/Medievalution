#include "../stdafx.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData* state_data)
	:State(state_data)
{
	updateEvents();
	loadEntities();
	StateManager::getInstance().addObserver(*this);
}

void MainMenuState::loadEntities()
{
	if (!language_rus.loadFromFile("resources/Flags/rus.png"))
	{
		LOG_WARN("File <<rus.png>> not found");
	}
	if (!language_eng.loadFromFile("resources/Flags/eng.png"))
	{
		LOG_WARN("File <<eng.png>> not found");
	}

	if (WindowSettings::getInstance().language == "rus")
	{
		localisation_file.open("translation/rus.json");
		localisation_file >> WindowSettings::getInstance().localisation;
		sprite.setTexture(language_rus);
	}

	if (WindowSettings::getInstance().language == "eng")
	{
		localisation_file.open("translation/eng.json");
		localisation_file >> WindowSettings::getInstance().localisation;
		sprite.setTexture(language_eng);
	}

	if (!background.loadFromFile("resources/Backgrounds/background_main.jpg"))
	{
		LOG_ERROR("File <<background.jpg>> not foubd");
	}

	shape.setSize(vec2f(StateManager::getInstance().state_data.window->getSize().x, StateManager::getInstance().state_data.window->getSize().y));
	shape.setTexture(&background);
}

MainMenuState::~MainMenuState()
{
	StateManager::getInstance().removeObserver(*this);
}

void MainMenuState::updateEvents(){}

void MainMenuState::updateImGui()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Main Menu",nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_new_game").c_str(), ImVec2(120, 0)))
	{
		StateManager::getInstance().changeState(std::make_shared <GameState>(state_data), false);
	}
	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_settings").c_str(), ImVec2(120, 0)))
	{
		StateManager::getInstance().changeState(std::make_shared <SettingsState>(state_data),false);
	}

	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_exit").c_str(), ImVec2(120, 0)))
	{
		endState();
	}

	static int counter = 0;
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x + 64;
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) 
	{
		sprite.setTexture(language_rus);
		 WindowSettings::getInstance().language = "rus";
		 WindowSettings::getInstance().saveToFile("config/graphic_settings.ini");

	}

	ImGui::SameLine(0.0f, spacing);

	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		sprite.setTexture(language_eng);
		WindowSettings::getInstance().language = "eng";
		WindowSettings::getInstance().saveToFile("config/graphic_settings.ini");
	}
	ImGui::Image(sprite, sf::Vector2f(120, 78));
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void MainMenuState::update(const float& dtime)			
{
	this->updateMousePositions();
}

void MainMenuState::render(sf::RenderTarget* target)							
{ 
	if (!target)
		target = this->window.get();
	target->setView(this->window->getDefaultView());
	target->draw(shape);
}

void MainMenuState::updateObserver()
{
	LOG_INFO("observer main menu update");
}