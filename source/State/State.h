#pragma once
#include "../Settings/WindowSettings.h"
#include "../Core/Core.h"
#include "../Observer/Observer.hpp"

class WindowSettings;
class State;

// Буферный класс для передачи инфы между стейтами
struct StateData
{
	std::stack <std::shared_ptr<State>> states;
	std::shared_ptr <sf::RenderWindow>	window;
	Jukebox jukebox = Jukebox("resources/Audio/Muisics/");
};


// Базовый класс состояний игры (главное меню, настройки и тд)
class State: public Observer
{
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

protected:
	StateData* stateData;
	std::stack< std::shared_ptr<State>> states;
	bool quit;
	bool replace;

public:
	std::shared_ptr <sf::RenderWindow> window;
};