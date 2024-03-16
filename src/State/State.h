#pragma once
#include "../Settings/WindowSettings.h"
#include "../Core/Core.h"

class WindowSettings;
class State;

// Буферный класс для передачи инфы между стейтами
struct StateData
{
	Jukebox jukebox = Jukebox("resources/Audio/Muisics/");
};


// Базовый класс состояний игры (главное меню, настройки и тд)
class State
{
public:
	State(StateData& state_data);
	virtual ~State();

	const bool& getQuit	() const;
	void		endState();

	virtual bool init()										= 0;
	virtual void updateEvents()								= 0;
	virtual void updateImGui()								= 0;
	virtual void update(const float& dtime)					= 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

protected:
	StateData* state_data;
	std::stack< std::shared_ptr<State>> states;
	bool quit;
	bool replace;
};