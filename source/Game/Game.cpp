#include "../stdafx.h"
#include "Game.h"

//------------------------------------------------------------------------------------------------------------------------
// инициализируем переменные
void Game::initVariables()                                                          
{
    core::music = std::make_unique<sf::Music>();
    core::dtime = 0.00f;                                                               // дельта времени 0.00f
    if (!music.openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
        LOG_ERROR ("fail sound");
	music.setLoop(true);
}

//------------------------------------------------------------------------------------------------------------------------
// инициализируем настройки графики
void Game::initGraphicSettings()
{
    this->graphicSettings.loadFromFIle("config/graphic_settings.ini");
}

//------------------------------------------------------------------------------------------------------------------------
// инициализируем окно приложения
void Game::initWindow()                                                             
{
    // если окно в режиме "во весь экран", то устанавливаем след. настройки
    if (this->graphicSettings.fullscrean)
        this->window = std::make_unique<sf::RenderWindow>
                                            (this->graphicSettings.resolution,
                                            this->graphicSettings.title, sf::Style::Fullscreen,
                                            this->graphicSettings.contextSettings);
   // если окнов оконном режиме, устанавливаем след. настройки
    else
        this->window = std::make_unique<sf::RenderWindow>   
                                            (this->graphicSettings.resolution,
                                            this->graphicSettings.title, sf::Style::Close,
                                            this->graphicSettings.contextSettings);
    
    this->window->setFramerateLimit         (this->graphicSettings.fps_limit);      // ограничиваем лимит fps, по-умолчанию 120 fps
    this->window->setVerticalSyncEnabled    (this->graphicSettings.vertycalSync);   // устанавливаем вертикальную синхронизацию, по-умолчанию выкл
}

//------------------------------------------------------------------------------------------------------------------------
// инициализируем буфер стейтов
void Game::initStateData()                                                         
{
     StateManager::getInstance().stateData.window = window;
     StateManager::getInstance().stateData.graphicSettings = &graphicSettings;                       // передаем данные о графикие
     StateManager::getInstance().stateData.states = &StateManager::getInstance().states;             // передаем данные о стейтах
}

//------------------------------------------------------------------------------------------------------------------------
// инициализируем стартовый стейт
void Game::initStates()                                                             
{
    StateManager::getInstance().init();
	ImGui::SFML::Init(*window);
    LOG_INFO ("Game started successful");
}

//------------------------------------------------------------------------------------------------------------------------
// конструткор
Game::Game()
{
    initVariables();                                                                // инициализируем переменные
    initGraphicSettings();                                                          // инициализируем настройки графики
    initWindow();                                                                   // инициализируем окно приложение
    initStateData();                                                                // инициализируем буфер стейтов
    initStates();                                                                   // инициализируем стейты
}

//------------------------------------------------------------------------------------------------------------------------
// деструктор
Game::~Game() 
{
    music.stop();
}

//------------------------------------------------------------------------------------------------------------------------
// обновление дельты времени
void Game::updateDeltaTime()                                                        
{
    core::dtime = this->m_clock.restart().asMilliseconds();                            // обновляем как милисекунды
}

//------------------------------------------------------------------------------------------------------------------------
// обвноялем события SFML
void Game::updateSFMLevents()                                                        
{
    while (this->window->pollEvent(core::sfmlEvent))                                    // пока крутится окно
    {
		ImGui::SFML::ProcessEvent(*window,core::sfmlEvent);
		if (core::sfmlEvent.type == sf::Event::Closed)                                  // если событие закрыть окно
		{
			this->window->close();                                                   // закрываем окно
		}
		if (!StateManager::getInstance().states.empty())                                                   // если в стеке есть стейты
		{
            StateManager::getInstance().states.back()->updateEvents();                                      // обновляем события sfml в активном стейте
		}
    }
}

//------------------------------------------------------------------------------------------------------------------------
// обновление логики
void Game::update()
{
    updateSFMLevents();                                                                     // обновляем события sfml
    if (!StateManager::getInstance().states.empty())                                        // если в стеке есть стейты
    {
        if (this->window->hasFocus()) {                                                     // если окно в фокусе
            ImGui::SFML::Update(*window, core::clock.restart());
            /*
            to do
            тут надо обновлять не последний(верхний) стейт, а активный!иначе получается стек
            StateManager::getInstance().getCurrentState()->update();
                    если колво стейтов изменилось, то обновляем и переходим к активному стейту
                    меню ----> загрузка ---> игра
                    (prev)      (curent)    (next)
            Дожидаюсь загрузки иры(next), делаю ее активным стейтом, закрываю предыдущий стейт
            */
            StateManager::getInstance().states.back()->update(core::dtime);                 // обновляем логику активного стейта
            StateManager::getInstance().states.back()->updateImGui();                       // обновление ImGui 
            if (StateManager::getInstance().states.back()->getQuit())                       // если выходим из активного стейта
            {
                StateManager::getInstance().states.back()->endState();                      // завершаем активный стейт в стеке
                delete   StateManager::getInstance().states.back();                         // очичаем от стейта память
                StateManager::getInstance().states.pop_back();                              // предпоследний стейт становится активным
                LOG_INFO("Count of state {}", StateManager::getInstance().states.size());
            }
        }
    }
    else
    {
        this->window->close();                                                      // закрываем окно приложения
    }

}

//------------------------------------------------------------------------------------------------------------------------
 // отрисовка
void Game::render()                                                                 
{
    this->window->clear(sf::Color(63, 72, 204));                                    // очистка экрана в синеватый цвет (63,72,204)
	if (!StateManager::getInstance().states.empty())														// если в стеке есть стейты
	{
        StateManager::getInstance().states.back()->render();												// отрисовка активного стейта
	}
	ImGui::SFML::Render(*window);
    this->window->display();														// выовд на экран
}

//------------------------------------------------------------------------------------------------------------------------
// запуск приложения
void Game::run()                                                                    
{
	if (core::music->openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
    {
        LOG_INFO ("Music track loaded sucsesfully");
	    core::music->play();
	    core::music->setLoop(true);
    }


    while (this->window->isOpen())                                                   // пока окно приложения открыто
    {
        updateDeltaTime();                                                           // обновление дельты времени
        update();                                                                    // обновление логики
        render();                                                                    // отрисовка
    }
	ImGui::SFML::Shutdown();
}
//------------------------------------------------------------------------------------------------------------------------