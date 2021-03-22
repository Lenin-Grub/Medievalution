#pragma once
#include "GraphicSettings.h"
#include "System.h"
#include "EntityManager.h"

class GraphicSettings;
class State;

class StateData															// буферный класс для передачи инфы между стейтами
{
public:
	StateData() {};

	GraphicSettings* graphicSettings;									// настройки окна приложения (размер, оконный режим и тд)
	std::stack<State*>* states;
	std::shared_ptr <sf::RenderWindow> window;

	unsigned short int fps_counter = 0;									// fps
	float sec_timer = 0.00;												// таймер для fps
};

class State
	:public System
{
protected:
	StateData* stateData;												// буферный класс для передачи инфы между стейтами
	std::stack<State*>* states;											// стэк состояний
	bool quit;															// проверка на выход из состояния
	std::shared_ptr <sf::RenderWindow> window;

	unsigned short int fps_counter = 0;									// fps									
	float sec_timer = 0.00;												// таймер для fps		
	sf::Text fpsText;													// вывод текста fps

public:
	EntityManager _entityManager;										// управление игровыми сущностями (игрок, враги и тд)

	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;										// проверка на выход из состояния
	void endState();													// выход из состояния

	virtual void updateMousePositions(sf::View* view = NULL);			// обновляем позиции мыши
	virtual void updateEvents() = 0;									// обновляем события sfml
	virtual void updateFPS();											// обновляем fps
	virtual void update(const float& dtime) = 0;						// обновление всей логики
	virtual void render(sf::RenderTarget* target = nullptr) = 0;		// отрисовка на экран
};