#pragma once
#include "../State/StateManager.h"

class Game
{
public:
	Game();
	virtual ~Game();
	void run();

private:
	void initGraphicSettings()	noexcept;
	void initWindow()			noexcept;
	void initStateData()		noexcept;
	void initStates()			noexcept;

	void initIcon()				noexcept;
	void initFonts()			noexcept;
	void initJukebox()			noexcept;

	void update();
	void render();
	void updateDeltaTime();
	void updateSFMLevents();

private:
	sf::Clock clock;
	std::shared_ptr <sf::RenderWindow> window;
};