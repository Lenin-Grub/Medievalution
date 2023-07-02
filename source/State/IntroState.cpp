#include "../stdafx.h"
#include "IntroState.h"

void IntroState::initEntities()
{
	shader.loadFromFile("shaders/test_shader.vert", "shaders/test_shader.frag");
	shape.setSize(sf::Vector2f(300, 300));
	sf::Texture texture;
	texture.loadFromFile("resources/intro.jpg");
	spr.setTexture(texture);
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
	//ImGui::Begin("Loading##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse /*| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/);
	//ImGui::SliderInt("Loading", &load, 0, 10, "%d%%");
	//ImGui::End();
}

void IntroState::update(const float& dtime)
{
	if (Input::isKeyReleased(sf::Keyboard::Key::Space))
	{
		//StateManager::getInstance().endState();
		StateManager::getInstance().addState(new MainMenuState(stateData));
	}

}

void IntroState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window.get();
	target->draw(spr);
}
