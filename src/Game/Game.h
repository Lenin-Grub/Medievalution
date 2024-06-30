#pragma once
#include "../State/IntroState.hpp"
#include "../State/StateMachine.hpp"
#include "../Settings/WindowSettings.h"


class Game
{
public:
	Game();
	virtual ~Game();
	void    run();

private:
	bool initGraphicSettings() noexcept;
	bool initWindow()          noexcept;
	bool initIcon()            noexcept;
	bool initFonts()           noexcept;
	bool initJukebox()         noexcept;
	void restartApplication();

private:
	std::shared_ptr <sf::RenderWindow> window;
	StateMachine     state_machine;
};