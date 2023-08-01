#pragma once
#include "../State/StateManager.h"

class Game
{
private:
	sf::Clock m_clock;						
	std::shared_ptr <sf::RenderWindow> window;
	sf::Music music;						
private:									
											
	void initVariables();					
	void initGraphicSettings();				
	void initWindow();						
	void initStateData();					
	void initStates();						
											
	void update();							
	void render();							
	void updateDeltaTime();					
	void updateSFMLevents();				
public:										
	Game();									
	virtual ~Game();						
	void run();								
};