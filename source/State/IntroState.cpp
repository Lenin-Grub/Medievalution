#include "../stdafx.h"
#include "IntroState.h"

IntroState::IntroState(StateData* state_data)
	:State(state_data)

{
	setBackground();
	StateManager::getInstance().addObserver(*this);
}

IntroState::~IntroState()
{
	StateManager::getInstance().removeObserver(*this);
}

void IntroState::setBackground()
{
	if (!background.loadFromFile("resources/Backgrounds/ruszastavka.png"))
	{
		LOG_ERROR("File <<ruszastavka.png>> not foubd");
	}

	shape.setSize(vec2f(StateManager::getInstance().state_data.window->getSize().x, StateManager::getInstance().state_data.window->getSize().y));
	shape.setTexture(&background);

	font.loadFromFile("resources/Fonts/Blackmoor.ttf");
	text.setFont(font);
	text.setString("MedievalutioN");
	text.setCharacterSize(core::math.convertToPercentage(StateManager::getInstance().state_data.window->getSize().x, 10));
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 0.4f);

	auto pos = StateManager::getInstance().state_data.window->getSize();
	text.setPosition(core::math.setCentre(pos.x,pos.y));
}

void IntroState::updateEvents(){}

void IntroState::updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::TextColored(ImVec4(1, 1, 1, 1), "Press \"SPACEBAR\" to continue");

	// Анимация и имитация загрузки
	static float progress = 0.0f, progress_dir = 1.0f;
	if (true)
	{
		progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;

		if (progress >= 1.0f)
		{
			StateManager::getInstance().changeState(std::make_shared <MainMenuState>(state_data), true);
		}

		if (progress >= +1.1f) 
		{
			progress = +1.1f; 
		}
	}

	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Text("Progress Bar");
	ImGui::End();
}


void IntroState::update(const float& dtime)
{
	if (Input::isKeyReleased(sf::Keyboard::Key::Space))
	{
		updateObserver();
		StateManager::getInstance().changeState(std::make_shared <MainMenuState>(state_data), true);
	}
}


void IntroState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window.get();
	target->draw(shape);
	target->draw(text);
}

void IntroState::updateObserver(){}