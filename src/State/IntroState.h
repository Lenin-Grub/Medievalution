#pragma once
#include "../State/State.h"
#include "../State/MainMenuState.h"
#include "../Observer/Observable.hpp"

class IntroState :
	public State
{
public:
	IntroState(StateData* state_data);
	void setBackground();
	virtual ~IntroState();

	void updateEvents() override;
	void updateImGui() override;
	void update(const float& dtime) override;
	void render(sf::RenderTarget* target = nullptr) override;

	void updateObserver() override;

private:
	int load = 0;

	sf::Texture background;
	sf::Sprite sprite;
	sf::RectangleShape shape;
	std::list<Observer*> observer_list;
	sf::Text text;
	sf::Font font;
};