#include "../stdafx.h"
#include "IntroState.h"


void IntroState::renderGUI(sf::RenderTarget& target)
{
}

IntroState::IntroState(StateData* state_data)
	:State(state_data)
{
	this->updateEvents();
	core::music->setVolume(WindowSettings::getInstance().music_volume);
	if (!background.loadFromFile("resources/Backgrounds/ruszastavka.png"))
	{
		LOG_ERROR("File <<ruszastavka.png>> not foubd");
	}

	sh.setSize(vec2f(StateManager::getInstance().stateData.window->getSize().x, StateManager::getInstance().stateData.window->getSize().y));
	sh.setTexture(&background);

	font.loadFromFile("resources/Fonts/Blackmoor.ttf");
	text.setFont(font);
	text.setString("MedievalutioN");
	text.setCharacterSize(200);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 0.4f);
	text.setPosition(vec2f(StateManager::getInstance().stateData.window->getSize().x / 2, StateManager::getInstance().stateData.window->getSize().y / 2));
	StateManager::getInstance().addObserver(*this);
}

IntroState::~IntroState()
{
	StateManager::getInstance().removeObserver(*this);
}

void IntroState::updateEvents()
{
}

void IntroState::updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Press \"SPACEBAR\" to continue");
	ImGui::End();
}

void IntroState::update(const float& dtime)
{
	if (Input::isKeyReleased(sf::Keyboard::Key::Space))
	{
		StateManager::getInstance().changeState(new MainMenuState(stateData),true);
	}
}

void IntroState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window.get();
	target->draw(sh);
	target->draw(text);
}

void IntroState::updateObserver()
{
	LOG_INFO("observer intro update");
}