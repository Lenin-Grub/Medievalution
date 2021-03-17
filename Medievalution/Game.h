#pragma once
#include "MainMenuState.h"


class Game :
	public S
{
private:
	GraphicSettings graphicSettings;				// настройки графики
	StateData stateData;							// передает в дочерние состояния информацию о времени, событиях и тд
	std::stack<State*> states;						// стэк состояния игры (главное меню, игра, настройки и тд)
	std::shared_ptr <sf::RenderWindow> window;		// окно
	sf::Clock m_clock;								// время

private:

	void initVariables();							// инициализация размера карты, окна, времени и тд
	void initGraphicSettings();						// загрузка настроек графики
	void initWindow();								// инициализация окна приложения
	void initStateData();							// data для обмена данными между состояниями
	void initStates();								// иницилизация состояния

public:
	Game();
	virtual ~Game();

	void endApplication();							// проверка на корректный выход программы
	void updateDeltaTime();							// обновление времени
	void updateSFMLevents();						// обновление событий сфмл

	void update();									// обновление
	void render();									// рендер
	void run();										// запуск приложения
};