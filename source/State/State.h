#pragma once
#include "../Settings/WindowSettings.h"
#include "../Core/Core.h"

//------------------------------------------------------------------------------------------------------------------------
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
	StateData* stateData;												// буферный класс для передачи инфы между стейтами
	std::list<State*>* states;											// стэк состояний
	bool quit;															// проверка на выход из состояния

public:
	std::shared_ptr <sf::RenderWindow> window;
public:
	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;										// проверка на выход из состояния
	void endState();													// выход из состояния

	virtual void updateMousePositions(sf::View* view = NULL);			// обновляем позиции мыши
	virtual void updateEvents() = 0;									// обновляем события sfml
	virtual void updateImGui() = 0;										// обновление ImGui
	virtual void update(const float& dtime) = 0;						// обновление всей логики
	virtual void render(sf::RenderTarget* target = nullptr) = 0;		// отрисовка на экран
};