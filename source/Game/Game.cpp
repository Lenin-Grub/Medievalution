#include "../stdafx.h"
#include "Game.h"

Game::Game()
{
    initGraphicSettings();
    initWindow();
    initIcon();
    initStateData();
    initStates();       
    initFonts();
    initJukebox();

    core::dtime = 0.00f;
}

Game::~Game() 
{
    while (!StateManager::getInstance().states.empty())
    {
        StateManager::getInstance().states.pop();
    }
}

void Game::run()
{
    while (this->window->isOpen())
    {
        updateDeltaTime();
        update();
        render();
    }
    ImGui::SFML::Shutdown();
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
         WindowSettings::getInstance().context_settings);
    // если окнов оконном режиме, устанавливаем след. настройки
    else
        this->window = std::make_unique<sf::RenderWindow>
        (WindowSettings::getInstance().resolution,
         WindowSettings::getInstance().title, sf::Style::Close,
         WindowSettings::getInstance().context_settings);

    this->window->setFramerateLimit(WindowSettings::getInstance().fps_limit);
    this->window->setVerticalSyncEnabled(WindowSettings::getInstance().vertycal_sync);
}

void Game::initStateData()
{
    StateManager::getInstance().state_data.window = window;
}

void Game::initStates()
{
    StateManager::getInstance().init();
    ImGui::SFML::Init(*window);
    LOG_INFO("Game started successful");
}

void Game::initIcon()
{
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

void Game::initFonts()
{
    //TODO перенсти в отдельное место
    //установка дефолтного шрифта в ImGui
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    config.GlyphMinAdvanceX = 6.0f;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.GlyphExtraSpacing.x = 10.0f;
    config.GlyphExtraSpacing.y = 10.0f;

    static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf",   20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenFontIcons.ttf",       20.f, &config, icon_ranges);
    io.Fonts->Build();

    ImGui::SFML::UpdateFontTexture();
    io.FontDefault = font1;
}

void Game::initJukebox()
{
    StateManager::getInstance().state_data.jukebox.requestAll();
    StateManager::getInstance().state_data.jukebox.setVolume(WindowSettings::getInstance().music_volume);
    StateManager::getInstance().state_data.jukebox.play();
}

void Game::updateDeltaTime()                                                        
{
    core::dtime = this->clock.restart().asMilliseconds();
}

void Game::updateSFMLevents()
{
    while (this->window->pollEvent(core::sfml_event))
    {
		ImGui::SFML::ProcessEvent(*window,core::sfml_event);
		if (core::sfml_event.type == sf::Event::Closed)
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
        if (this->window->hasFocus()) 
        {                                             
            ImGui::SFML::Update(*window, core::clock.restart());

            StateManager::getInstance().states.top()->update(core::dtime);
            StateManager::getInstance().states.top()->updateImGui();
            StateManager::getInstance().state_data.jukebox.update();

            if (StateManager::getInstance().states.top()->getQuit())
            {
                StateManager::getInstance().states.top()->endState();
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
    //this->window->clear(sf::Color(63, 72, 204));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!StateManager::getInstance().states.empty())
    {
        window->pushGLStates();
        StateManager::getInstance().states.top()->render();
        window->popGLStates();
    }
	ImGui::SFML::Render(*window);
    this->window->display();
}