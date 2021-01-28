#pragma once
#include "GraphicSettings.h"


class GraphicSettings;
class State;

class StateData
{
public:
	StateData() {};

	GraphicSettings* graphicSettings;									// настройки графики
	std::stack<State*>* states;											// состояния
	sf::RenderWindow* window;											// окно

	unsigned short int fps_counter = 0;									// счетчик fps
	float sec_timer = 0.00;												// таймер в секундах
};

class State
	:public S
{
protected:
	StateData* stateData;												// передает в дочерние состояния информацию о времени, событиях и тд
	std::stack<State*>* states;											// стек  всех состояний (главное меню, экран настроек, редактор карт и тд)
	bool quit;															// вышли ли из программы?
	sf::RenderWindow* window;											// окно

	unsigned short int fps_counter = 0;									// счетчик fps
	float sec_timer = 0.00;												// таймер в секундах
	sf::Text fpsText;													// fps
public:
	State(StateData* state_data);
	virtual ~State();

	const bool& getQuit() const;										// завершение приложения
//	const bool getKeytime();											// задержка отклика клавиатуры
	void endState();													// выход из состояния игры (например из игры в главное меню)

	virtual void updateMousePositions(sf::View* view = NULL);			// обвноление позиции мыши
	//virtual void updateKeytime(const float& dtime);						// обновление отклика клавиатуры
	//virtual void updateInput(const float& dtime) = 0;					// обновление нажатий
	virtual void updateEvents() = 0;
	virtual void update(const float& dtime) = 0;						// обновление

	virtual void render(sf::RenderTarget* target = nullptr) = 0;		// рендер
};