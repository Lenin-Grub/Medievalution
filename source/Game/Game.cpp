#include "../stdafx.h"
#include "Game.h"

void Game::initVariables()                                                          
{
    //TODO музыка должна быть в другом месте
    core::music = std::make_unique<sf::Music>();
    core::dtime = 0.00f;
    if (!music.openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
        LOG_ERROR ("Music <<Now-We-Ride.wav>> not found");
	music.setLoop(true);
}

void Game::initGraphicSettings()
{
    try
    {
        WindowSettings::getInstance().loadFromFIle("config/settings.json");
    }
    catch (const std::exception& ex)
    {
       LOG_ERROR("Settings.json not found!");
       LOG_ERROR(ex.what());
    }
}

void Game::initWindow()
{
    // если окно в режиме "во весь экран", то устанавливаем след. настройки
    if (WindowSettings::getInstance().fullscrean)
        this->window = std::make_unique<sf::RenderWindow>
        (WindowSettings::getInstance().resolution,
            WindowSettings::getInstance().title, sf::Style::Fullscreen,
            WindowSettings::getInstance().contextSettings);
    // если окнов оконном режиме, устанавливаем след. настройки
    else
        this->window = std::make_unique<sf::RenderWindow>
        (WindowSettings::getInstance().resolution,
            WindowSettings::getInstance().title, sf::Style::Close,
            WindowSettings::getInstance().contextSettings);

    this->window->setFramerateLimit(WindowSettings::getInstance().fps_limit);      
    this->window->setVerticalSyncEnabled(WindowSettings::getInstance().vertycalSync);   

    //TODO перенести в отдельное место
    sf::Image i;
    if (i.loadFromFile("resources/Icons/icon.png"))
    {
        this->window->setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
    }
    else
    {
        LOG_ERROR("File <<icon.png>> not loadded");
    }
}

void Game::initStateData()                                                         
{
     StateManager::getInstance().stateData.window = window;
}

void Game::initStates()                                                             
{
    StateManager::getInstance().init();
	ImGui::SFML::Init(*window);

    //TODO перенсти в отдельное место
    //установка дефолтного шрифта в ImGui

    ImGuiIO& io = ImGui::GetIO();
    ImFont* f = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf",20.f,NULL,io.Fonts->GetGlyphRangesCyrillic());
    ImGui::SFML::UpdateFontTexture();
    ImFont* font = io.Fonts->Fonts[1];
    io.FontDefault = font;

    LOG_INFO ("Game started successful");
}

Game::Game()
{
    initVariables();                                                                
    initGraphicSettings();                                                          
    initWindow();                                                                   
    initStateData();                                                                
    initStates();                                                                   
}

Game::~Game() 
{
    music.stop();
}

void Game::updateDeltaTime()                                                        
{
    core::dtime = this->m_clock.restart().asMilliseconds();                         
}

void Game::updateSFMLevents()                                                       
{
    while (this->window->pollEvent(core::sfmlEvent))                                
    {
		ImGui::SFML::ProcessEvent(*window,core::sfmlEvent);
		if (core::sfmlEvent.type == sf::Event::Closed)                              
		{
			this->window->close();                                                  
		}
		if (!StateManager::getInstance().states.empty())                            
		{
            StateManager::getInstance().states.top()->updateEvents();               
		}
    }
}

void Game::update()
{
    updateSFMLevents();                                                             
    if (!StateManager::getInstance().states.empty())                                
    {
        if (this->window->hasFocus()) {                                             
            ImGui::SFML::Update(*window, core::clock.restart());
            StateManager::getInstance().states.top()->update(core::dtime);          
            StateManager::getInstance().states.top()->updateImGui();                
            if (StateManager::getInstance().states.top()->getQuit())                
            {
                StateManager::getInstance().states.top()->endState();               
                delete   StateManager::getInstance().states.top();                  
                StateManager::getInstance().states.pop();                           
                LOG_INFO("Count of state {}", StateManager::getInstance().states.size());
            }
        }
    }
    else
    {
        this->window->close();                                                      
    }
}

void Game::render()                                                                 
{
    this->window->clear(sf::Color(63, 72, 204));                                    
	if (!StateManager::getInstance().states.empty())								
	{
        StateManager::getInstance().states.top()->render();						    
	}
	ImGui::SFML::Render(*window);
    this->window->display();														
}

void Game::run()                                                                    
{
	if (core::music->openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
    {
        LOG_INFO ("Music track loaded sucsesfully");
	    core::music->play();
	    core::music->setLoop(true);
    }
    while (this->window->isOpen())                                                  
    {
        updateDeltaTime();                                                          
        update();                                                                   
        render();                                                                   
    }
	ImGui::SFML::Shutdown();
}