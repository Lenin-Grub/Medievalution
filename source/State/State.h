#pragma once
#include "../Settings/WindowSettings.h"
#include "../Core/Core.h"

class WindowSettings;
class State;


//------------------------------------------------------------------------------------------------------------------------
// буферный класс для передачи инфы между стейтами
class StateData
{
public:
	StateData() = default;
	virtual ~StateData() = default;

	std::list<State*>* states;
	std::shared_ptr <sf::RenderWindow> window;
};


//------------------------------------------------------------------------------------------------------------------------
// базовый класс состояний игры (главное меню, настройки и тд)
class State
{
protected:
	StateData* stateData;										
	std::list<State*>* states;									
	bool quit;													

public:
	std::shared_ptr <sf::RenderWindow> window;
public:
	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;								
	void endState();											

	virtual void updateMousePositions(sf::View* view = NULL);	
	virtual void updateEvents() = 0;							
	virtual void updateImGui() = 0;								
	virtual void update(const float& dtime) = 0;				
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};