#pragma once
#include "../Settings/WindowSettings.h"
#include "../Core/Core.h"
#include "../Observer/Observer.hpp"

class WindowSettings;
class State;


//------------------------------------------------------------------------------------------------------------------------
// буферный класс для передачи инфы между стейтами
class StateData
{
public:
	StateData() {};
	virtual ~StateData() = default;

	std::stack<State*>* states;
	std::shared_ptr <sf::RenderWindow> window;
};


//------------------------------------------------------------------------------------------------------------------------
// базовый класс состояний игры (главное меню, настройки и тд)
class State: public Observer
{
protected:
	StateData* stateData;
	std::stack<State*>* states;
	bool quit;
	bool replace;

public:
	std::shared_ptr <sf::RenderWindow> window;
public:
	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;
	void endState();

	virtual void updateMousePositions(sf::View* view = nullptr);
	virtual void updateEvents() = 0;
	virtual void updateImGui() = 0;
	virtual void update(const float& dtime) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

	virtual void updateObserver() = 0;
};