#pragma once
#include "../State/MainMenuState.h"
#include "../State/GameState.h"
#include "../State/SettingsState.h"
#include "../State/IntroState.h"


//------------------------------------------------------------------------------------------------------------------------
// Основной класс игра, отвечающий за запуск,закрытие, обновление игры
class Game
{
//------------------------------------------------------------------------------------------------------------------------
private:
	//WindowSettings graphicSettings;					// настройки графики
	sf::Clock m_clock;								// время
	std::shared_ptr <sf::RenderWindow> window;		// окно
	sf::Music music;
//------------------------------------------------------------------------------------------------------------------------
private:

	void initVariables();							// инициализация размера карты, окна, времени и тд
	void initGraphicSettings();						// загрузка настроек графики
	void initWindow();								// инициализация окна приложения
	void initStateData();							// data для обмена данными между состояниями
	void initStates();								// иницилизация состояния
	
	void update();									// обновление
	void render();									// рендер
	void updateDeltaTime();							// обновление времени
	void updateSFMLevents();						// обновление событий сфмл
//------------------------------------------------------------------------------------------------------------------------
public:
	Game();
	virtual ~Game();
	void run();										// запуск приложения
};