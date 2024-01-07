#pragma once
#include "../State/StateManager.h"
#include "../Resource/Jukebox.hpp"

class Game
{
public:
	Game();
	virtual ~Game();
	void run();

private:										
	void initGraphicSettings();
	void initWindow();
	void initStateData();
	void initStates();
											
	void update();
	void render();
	void updateDeltaTime();
	void updateSFMLevents();

private:
	sf::Clock m_clock;
	std::shared_ptr <sf::RenderWindow> window;
	Jukebox jukebox;
};