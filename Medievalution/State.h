#pragma once
#include "GraphicSettings.h"
#include "System.h"

class GraphicSettings;
class State;

class StateData
{
public:
	StateData() {};

	GraphicSettings* graphicSettings;
	std::stack<State*>* states;
	std::shared_ptr <sf::RenderWindow> window;

	unsigned short int fps_counter = 0;
	float sec_timer = 0.00;
};

class State
	:public System
{
protected:
	StateData* stateData;
	std::stack<State*>* states;
	bool quit;
	std::shared_ptr <sf::RenderWindow> window;

	unsigned short int fps_counter = 0;									
	float sec_timer = 0.00;												
	sf::Text fpsText;

public:
	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;
	void endState();

	virtual void updateMousePositions(sf::View* view = NULL);
	virtual void updateEvents() = 0;
	virtual void update(const float& dtime) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};