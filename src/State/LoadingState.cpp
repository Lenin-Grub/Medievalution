#include "../stdafx.h"
#include "StateMachine.hpp"
#include "LoadingState.hpp"


LoadingState::LoadingState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{ data, machine, window, replace }
, alpha{ 255, 255, 255, 255 }
{
	LOG_INFO("State Loading\t Init");
	setBackground();
}

void LoadingState::onDeactivate()
{
	LOG_INFO("State Loading\t Deactivate");
}

void LoadingState::onActivate()
{
	LOG_INFO("State Loading\t Activate"); 
}

void LoadingState::updateEvents()
{
}

void LoadingState::updateImGui()
{
}

void LoadingState::update(const float& dtime)
{
}

void LoadingState::draw(sf::RenderTarget* target)
{
	std::thread thread([&]()
		{
			auto load_state = StateMachine::build<GameState>(data, state_machine, window, true);
			if (load_state->isLoad())
			{
				next_state = std::move(load_state);
			}
		});

	window.clear();
	window.draw(shape);
	window.draw(text);
	ImGui::SFML::Render(window);
	window.display();

	thread.join();
}

void LoadingState::setBackground()
{
	if (!background.loadFromFile("resources/Backgrounds/ruszastavka.png"))
	{
		LOG_ERROR("File \"background\" not foubd");
	}
	if (!font.loadFromFile("resources/Fonts/Blackmoor.ttf"))
	{
		LOG_ERROR("File \"font\" not foubd");
	}

	shape.setTexture(&background);
	shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

	text.setFont(font);
	text.setString("Loading");
	text.setCharacterSize(core::math.convertToPercentage(window.getSize().x, 10));
	text.setFillColor(alpha);

	sf::FloatRect textRect = text.getLocalBounds();

	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 0.4f);
	text.setPosition(core::math.setCentre(window.getSize().x, window.getSize().y));
}
