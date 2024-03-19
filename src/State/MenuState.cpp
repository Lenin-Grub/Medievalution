#include "../stdafx.h"
#include "StateMachine.hpp"
#include "MenuState.hpp"

MenuState::MenuState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{data, machine, window, replace }
{
	LOG_INFO("State Menu\t Init");
	setBackground();
	initLocalisation();
}

void MenuState::pause()
{
	LOG_INFO("State Menu\t Pause");
}

void MenuState::resume()
{
	LOG_INFO("State Menu\t Resume");
}

void MenuState::updateEvents()
{
	if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		state_machine.quit();
	}
}

void MenuState::updateImGui()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar
									 | ImGuiWindowFlags_NoCollapse
									 | ImGuiWindowFlags_AlwaysAutoResize
									 | ImGuiWindowFlags_NoResize
									 | ImGuiWindowFlags_NoMove);

	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_new_game").c_str(), ImVec2(120, 0)))
	{
		m_next = StateMachine::build<GameState>(data, state_machine, window, false);
	}

	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_settings").c_str(), ImVec2(120, 0)))
	{
		m_next = StateMachine::build<SettingsState>(data, state_machine, window, false);
	}

	if (ImGui::Button(Localisation::getInstance().getStringByKey("T_exit").c_str(), ImVec2(120, 0)))
	{
		state_machine.quit();
	}
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void MenuState::update(const float& dtime)
{

}

void MenuState::draw(sf::RenderTarget* target)
{
	window.clear();

	if (!target)
		target = &window;
	target->setView(window.getDefaultView());

	window.draw(shape);

	ImGui::SFML::Render(window);

	window.display();
}


void MenuState::setBackground()
{
	if (!background.loadFromFile("resources/Backgrounds/background_main.jpg"))
	{
		LOG_ERROR("File \"background_main.jpg\" not foubd");
	}

	shape.setTexture(&background);
	shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
}

void MenuState::initLocalisation()
{

	if (WindowSettings::getInstance().language == "rus")
	{
		localisation_file.open("translation/rus.json");
		localisation_file >> WindowSettings::getInstance().localisation;
	}

	if (WindowSettings::getInstance().language == "eng")
	{
		localisation_file.open("translation/eng.json");
		localisation_file >> WindowSettings::getInstance().localisation;
	}
}