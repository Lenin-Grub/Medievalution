#include "stdafx.h"
#include "Game.h"

void Game::initVariables()
{
    this->window = NULL;
    S::dtime = 0.00f;
}

void Game::initGraphicSettings()
{
    this->graphicSettings.loadFromFIle("Config/graphic_settings.ini");
}

void Game::initWindow()
{
    if (this->graphicSettings.fullscrean)
    
        this->window = std::make_unique<sf::RenderWindow>   (this->graphicSettings.resolution,
                                                             this->graphicSettings.title, sf::Style::Fullscreen,
                                                             this->graphicSettings.contextSettings);
    
    else
    
        this->window = std::make_unique<sf::RenderWindow>   (this->graphicSettings.resolution,
                                                             this->graphicSettings.title, sf::Style::Close,
                                                             this->graphicSettings.contextSettings);
    
    this->window->setFramerateLimit(this->graphicSettings.fps_limit);
    this->window->setVerticalSyncEnabled(this->graphicSettings.vertycalSync);
}

void Game::initStateData()
{
    this->stateData.window = this->window;
    this->stateData.graphicSettings = &this->graphicSettings;
    this->stateData.states = &this->states;
}

void Game::initStates()
{
    this->states.push(new MainMenuState(&this->stateData));
    cout << "Debug:: ¬ы зашли в главное меню" << endl;
}

Game::Game()
{
    initVariables();
    initGraphicSettings();
    initWindow();
    initStateData();
    initStates();
}

Game::~Game() {}

void Game::endApplication()
{
    cout << "Debug:: вы вышли из приложени€" << endl;
}

void Game::updateDeltaTime()
{
    S::dtime = this->m_clock.restart().asMilliseconds();
}

void Game::updateSFMLevents()
{
    while (this->window->pollEvent(S::sfmlEvent))
    {
		if (S::sfmlEvent.type == sf::Event::Closed)
			this->window->close();
		if (!this->states.empty())
		{
			this->states.top()->updateEvents();
		}
    }
}

void Game::update()
{
    updateSFMLevents();
    if (!this->states.empty())
    {
        if (this->window->hasFocus()) {
            this->states.top()->update(S::dtime);
            if (this->states.top()->getQuit())
            {
                this->states.top()->endState();
                delete this->states.top();
                this->states.pop();
            }
        }
    }
    else
    {
        this->endApplication();
        this->window->close();
    }
}

void Game::render()
{
    this->window->clear(sf::Color(63, 72, 204));
    if (!this->states.empty())
        this->states.top()->render();
    this->window->display();
}

void Game::run()
{
    while (this->window->isOpen())
    {
        updateDeltaTime();
        update();
        render();
    }
}