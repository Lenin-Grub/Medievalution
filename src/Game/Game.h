#pragma once
#include "../State/StateManager.h"
#include "../State/IntroState.hpp"
#include "../State/MainMenuState.hpp"


class Game
{
public:
	Game();
	virtual ~Game();
	void run();

private:
	bool initGraphicSettings()	noexcept;
	bool initWindow()			noexcept;
	
	bool initIcon()				noexcept;
	void initFonts()			noexcept;
	bool initJukebox()			noexcept;

	void update();
	void render();
	void updateSFMLevents();

private:
	sf::Clock clock;
	std::shared_ptr <sf::RenderWindow> window;
	StateMachine state_manager;
};