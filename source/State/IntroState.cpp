#include "../stdafx.h"
#include "IntroState.h"

void IntroState::initEntities()
{
	sf::Texture texture;
	if (texture.loadFromFile("resources/intro.jpg"))
	{
		spr.setTexture(texture);
	}
	else
	{
		LOG_ERROR("File <<intro.jpg>> not found");
	}
}

void IntroState::renderGUI(sf::RenderTarget& target)
{
}

IntroState::IntroState(StateData* state_data)
	:State(state_data)
{
	initEntities();
}

IntroState::~IntroState()
{
}

void IntroState::updateEvents()
{
}

void IntroState::updateImGui()
{
}

void IntroState::update(const float& dtime)
{
	if (Input::isKeyReleased(sf::Keyboard::Key::Space))
	{
		StateManager::getInstance().addState(new MainMenuState(stateData));
	}
}

void IntroState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window.get();
	target->draw(spr);
}
