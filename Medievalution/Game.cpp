#include "stdafx.h"
#include "Game.h"

void Game::initVariables()                                          // инициализируем переменные
{
    this->window = NULL;                                            // окно приложения NULL
    S::dtime = 0.00f;                                               // дельта времени 0.00f
}

void Game::initGraphicSettings()                                    // инициализируем настройки графики
{
    this->graphicSettings.loadFromFIle("Config/graphic_settings.ini");
}

void Game::initWindow()                                             // инициализируем окно приложения
{
    if (this->graphicSettings.fullscrean)                           // если окно в режиме "во весь экран", то устанавливаем след. настройки
    
        this->window = std::make_unique<sf::RenderWindow>   (this->graphicSettings.resolution,
                                                             this->graphicSettings.title, sf::Style::Fullscreen,
                                                             this->graphicSettings.contextSettings);
                                                                    // если окнов оконном режиме, устанавливаем след. настройки
    else
    
        this->window = std::make_unique<sf::RenderWindow>   (this->graphicSettings.resolution,
                                                             this->graphicSettings.title, sf::Style::Close,
                                                             this->graphicSettings.contextSettings);
    
    this->window->setFramerateLimit(this->graphicSettings.fps_limit);               // ограничиваем лимит fps, по-умолчанию 120 fps
    this->window->setVerticalSyncEnabled(this->graphicSettings.vertycalSync);       // устанавливаем вертикальную синхронизацию, по-умолчанию выкл
}

void Game::initStateData()                                              // инициализируем буфер стейтов
{
    this->stateData.window = this->window;                              // передаем данные об окне в буфер
    this->stateData.graphicSettings = &this->graphicSettings;           // передаем данные о графикие
    this->stateData.states = &this->states;                             // передаем данные о стейтах
}

void Game::initStates()                                                 // инициализируем стартовый стейт
{
    this->states.push(new MainMenuState(&this->stateData));             // создаем стартовый стейт приложения "Главное меню"
    cout << "Debug:: Вы зашли в главное меню" << endl;
}

Game::Game()
{
    initVariables();                                                  // инициализируем переменные
    initGraphicSettings();                                            // инициализируем настройки графики
    initWindow();                                                     // инициализируем окно приложение
    initStateData();                                                  // инициализируем буфер стейтов
    initStates();                                                     // инициализируем стейты
}

Game::~Game() {}

void Game::endApplication()                                          // тестовая проверка завершения стейта
{
    cout << "Debug:: вы вышли из приложения" << endl;
}

void Game::updateDeltaTime()                                        // обновляем дельту времени
{
    S::dtime = this->m_clock.restart().asMilliseconds();            // обновляем как милисекунды
}

void Game::updateSFMLevents()                                       // обвноялем события SFML
{
    while (this->window->pollEvent(S::sfmlEvent))                   // пока крутится окно
    {
		if (S::sfmlEvent.type == sf::Event::Closed)                 // если событие закрыть окно
			this->window->close();                                  // закрываем окно
		if (!this->states.empty())                                  // если в стеке есть стейты
		{
			this->states.top()->updateEvents();                     // обновляем события sfml в активном стейте
		}
    }
}

void Game::update()
{
    updateSFMLevents();                                             // обновляем события sfml
    if (!this->states.empty())                                      // если в стеке есть стейты
    {
        if (this->window->hasFocus()) {                             // если окно в фокусе
            this->states.top()->update(S::dtime);                   // обновляем логику активного стейта
            if (this->states.top()->getQuit())                      // если выходим из активного стейта
            {
                this->states.top()->endState();                     // завершаем активный стейт в стеке
                delete this->states.top();                          // очичаем от стейта память
                this->states.pop();                                 // предпоследний стейт становится активным
            }
        }
    }
    else
    {
        this->endApplication();                                    // вывод проверки на закрытие окна
        this->window->close();                                     // закрываем окно приложения
    }
}

void Game::render()                                                 // отрисовка
{
    this->window->clear(sf::Color(63, 72, 204));                    // очистка экрана в синеватый цвет (63,72,204)
    if (!this->states.empty())                                      // если в стеке есть стейты
        this->states.top()->render();                               // отрисовка активного стейта
    this->window->display();                                        // выовд на экран
}

void Game::run()                                                    // запуск приложения
{
    while (this->window->isOpen())                                  // пока окно приложения открыто
    {
        updateDeltaTime();                                          // обновление дельты времени
        update();                                                   // обновление логики
        render();                                                   // отрисовка
    }
}