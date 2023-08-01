#include "../stdafx.h"
#include "Game.h"

void Game::initVariables()                                                          
{
    //TODO музыка должна быть в другом месте
    core::music = std::make_unique<sf::Music>();
    core::dtime = 0.00f;
    if (!music.openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
        LOG_ERROR ("fail sound");
	music.setLoop(true);
}

void Game::initGraphicSettings()
{
    //TODO сделать проверку
    WindowSettings::getInstance().loadFromFIle("config/settings.json");
}

void Game::initWindow()                                                             
{
    // если окно в режиме "во весь экран", то устанавливаем след. настройки
    if (WindowSettings::getInstance().fullscrean)
        this->window = std::make_unique<sf::RenderWindow>
                                            (   WindowSettings::getInstance().resolution,
                                                WindowSettings::getInstance().title, sf::Style::Fullscreen,
                                                WindowSettings::getInstance().contextSettings);
   // если окнов оконном режиме, устанавливаем след. настройки
    else
        this->window = std::make_unique<sf::RenderWindow>   
                                            (    WindowSettings::getInstance().resolution,
                                                 WindowSettings::getInstance().title, sf::Style::Close,
                                                 WindowSettings::getInstance().contextSettings);
    
    this->window->setFramerateLimit         (WindowSettings::getInstance().fps_limit);      // ограничиваем лимит fps, по-умолчанию 120 fps
    this->window->setVerticalSyncEnabled    (WindowSettings::getInstance().vertycalSync);   // устанавливаем вертикальную синхронизацию, по-умолчанию выкл
    
    //иконка приложения
    //TODO сделать отдельно и добавить проверку
    sf::Image i;
    i.loadFromFile("resources/Icons/icon.png");
   this->window->setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
}

void Game::initStateData()                                                         
{
     StateManager::getInstance().stateData.window = window;
}

void Game::initStates()                                                             
{
    StateManager::getInstance().init();
	ImGui::SFML::Init(*window);

    //установка дефолтного шрифта в ImGui
    //TODO перенсти в отдельное место

    ImGuiIO& io = ImGui::GetIO();
    ImFont* f = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf",20.f,NULL,io.Fonts->GetGlyphRangesCyrillic());
    ImGui::SFML::UpdateFontTexture();
    ImFont* font = io.Fonts->Fonts[1];
    io.FontDefault = font;

    LOG_INFO ("Game started successful");
}

Game::Game()
{
    initVariables();                                                                // инициализируем переменные
    initGraphicSettings();                                                          // инициализируем настройки графики
    initWindow();                                                                   // инициализируем окно приложение
    initStateData();                                                                // инициализируем буфер стейтов
    initStates();                                                                   // инициализируем стейты
}

Game::~Game() 
{
    music.stop();
}

void Game::updateDeltaTime()                                                        
{
    core::dtime = this->m_clock.restart().asMilliseconds();                            // обновляем как милисекунды
}

void Game::updateSFMLevents()                                                        
{
    while (this->window->pollEvent(core::sfmlEvent))                                 // пока крутится окно
    {
		ImGui::SFML::ProcessEvent(*window,core::sfmlEvent);
		if (core::sfmlEvent.type == sf::Event::Closed)                               // если событие закрыть окно
		{
			this->window->close();                                                   // закрываем окно
		}
		if (!StateManager::getInstance().states.empty())                             // если в стеке есть стейты
		{
            StateManager::getInstance().states.top()->updateEvents();               // обновляем события sfml в активном стейте
		}
    }
}

void Game::update()
{
    updateSFMLevents();                                                                     // обновляем события sfml
    if (!StateManager::getInstance().states.empty())                                        // если в стеке есть стейты
    {
        if (this->window->hasFocus()) {                                                     // если окно в фокусе
            ImGui::SFML::Update(*window, core::clock.restart());
            StateManager::getInstance().states.top()->update(core::dtime);                 // обновляем логику активного стейта
            StateManager::getInstance().states.top()->updateImGui();                       // обновление ImGui 
            if (StateManager::getInstance().states.top()->getQuit())                       // если выходим из активного стейта
            {
                StateManager::getInstance().states.top()->endState();                      // завершаем активный стейт в стеке
                delete   StateManager::getInstance().states.top();                         // очичаем от стейта память
                StateManager::getInstance().states.pop();                              // предпоследний стейт становится активным
                LOG_INFO("Count of state {}", StateManager::getInstance().states.size());
            }
        }
    }
    else
    {
        this->window->close();                                                      // закрываем окно приложения
    }
}

void Game::render()                                                                 
{
    this->window->clear(sf::Color(63, 72, 204));                                    // очистка экрана в синеватый цвет (63,72,204)
	if (!StateManager::getInstance().states.empty())								// если в стеке есть стейты
	{
        StateManager::getInstance().states.top()->render();						    // отрисовка активного стейта
	}
	ImGui::SFML::Render(*window);
    this->window->display();														// выовд на экран
}

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