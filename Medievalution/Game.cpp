#include "stdafx.h"
#include "Game.h"

void Game::initVariables()
{
	this->window = NULL;																// стартовое окно
	S::dtime = 0.f;																		// начало времени
//	S::gridSize = 32;																	// размер тайла
//	S::mapSize = 10;																	// размер карты
}

void Game::initGraphicSettings()														// загрузка настроек графики
{
	this->graphicSettings.loadFromFIle("Config/graphic_settings.ini");
}

void Game::initWindow()																	// инициализация окна приложения
{
	if (this->graphicSettings.fullscrean)												// настройки окна если полный экран
		this->window = new sf::RenderWindow(this->graphicSettings.resolution,
			this->graphicSettings.title, sf::Style::Fullscreen,
			this->graphicSettings.contextSettings);
	else
		this->window = new sf::RenderWindow(this->graphicSettings.resolution,			// настройки окна если оконный режим
			this->graphicSettings.title, sf::Style::Close,
			this->graphicSettings.contextSettings);

	this->window->setFramerateLimit(this->graphicSettings.fps_limit);					// лимит кадров по умолчанию 120
	this->window->setVerticalSyncEnabled(this->graphicSettings.vertycalSync);			// вертикальная синхронизация по умолчанию выкл
}

void Game::initStateData()																// инициализация состояний игры в буферный data класс для удобной передачи данных между состояниями
{
	this->stateData.window = this->window;
	this->stateData.graphicSettings = &this->graphicSettings;
	this->stateData.states = &this->states;
}

void Game::initStates()																	// инициализация состояний игры (главное меню, игра, настройки)
{
	this->states.push(new MainMenuState(&this->stateData));								//инициализация главного меню
	cout << "Debug::Вы зашли в главное меню" << endl;
}

Game::Game()
{
	this->initVariables();																// инициализируем переменные
	this->initGraphicSettings();														// инициализируем настройки графики
	this->initWindow();																	// инициализируем окно
	this->initStateData();																// инициализируем data-буфер состояний
	this->initStates();																	// инициализируем состояния
	//fonts = Fonts();																	// загрузка шрифтов
	//textures = Textures();																// загрузка текстур
	//audio = Audio();																	// загрузка звуков
}

Game::~Game() { delete this->window; }													// очищаем память от окон

void Game::endApplication() { cout << "Debug:: Программа завершена" << endl; }			// проверка корректного выхода из программы

void Game::updateDeltaTime() { dtime = this->m_clock.restart().asMilliseconds(); } 		// обновление времени в миллисекундах

void Game::updateSFMLevents()															// обновление событий sfml
{
	while (this->window->pollEvent(S::sfmlEvent))										// пока открыто приложение, обновляем sfml
	{
		if (S::sfmlEvent.type == sf::Event::Closed)										// закрываем окно
			this->window->close();
		if (!this->states.empty())
		{
			this->states.top()->updateEvents();
		}
	//	if (!this->states.empty())															// обновляем состояния, если они есть в стеке
	//	{
	//		if (this->window->hasFocus()) {
	//			this->states.top()->update(S::dtime);										// обновляем активное состояние за единицу времени
	//			if (this->states.top()->getQuit())											// проверяем выход из приложения
	//			{
	//				this->states.top()->endState();											// выходим из состояния
	//				delete this->states.top();												// очищаем последнее состояние(пример вышли из состояния игры в главное меню, удаляем состояние игры)
	//				this->states.pop();
	//			}
	//		}
	//	}
	//	else
	//	{
	//		this->endApplication();															// выводим в консоль успешное завершение программы
	//		this->window->close();															// закрываем окно приложения
	//	}
	}
}

void Game::update()																		// обновление
{
	this->updateSFMLevents();															// обновляем sfml события
	if (!this->states.empty())															// обновляем состояния, если они есть в стеке
	{
		if (this->window->hasFocus()) {
			this->states.top()->update(S::dtime);										// обновляем активное состояние за единицу времени
			if (this->states.top()->getQuit())											// проверяем выход из приложения
			{
				this->states.top()->endState();											// выходим из состояния
				delete this->states.top();												// очищаем последнее состояние(пример вышли из состояния игры в главное меню, удаляем состояние игры)
				this->states.pop();
			}
		}
	}
	else
	{
		this->endApplication();															// выводим в консоль успешное завершение программы
		this->window->close();															// закрываем окно приложения
	}
}

void Game::render()
{
	this->window->clear(sf::Color(63, 72, 204));										// очищаем экран
	if (!this->states.empty())															// если стэк состояний(главное меню, игра и тд) не пустой, то делаем отрисовку
		this->states.top()->render();													// отрисовываем только активное состояние (другие в этот момент не отрисовываются)
	this->window->display();															// выводим на экран				
}

void Game::run()
{
	while (this->window->isOpen())														//бесконечный цикл, пока окно открыто
	{
		this->updateDeltaTime();														// обновляем время
		this->update();																	// обвноляем игру
		this->render();																	// отрисовываем игру
	}
}
