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

void IntroState::addObserver(Observer& observer)
{
	observer_list.push_back(&observer);
}

void IntroState::removeObserver(Observer& observer)
{
	observer_list.remove(&observer);
}

void IntroState::notifyObservers()
{
	for (auto o : observer_list)
	{
		o->updateObserver();
	}
	//std::list<Observer*>::iterator iterator = observer_list.begin();
	//while (iterator != observer_list.end()) 
	//{
	//	(*iterator)->updateObserver();
	//	++iterator;
	//}
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
