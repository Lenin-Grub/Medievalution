#pragma once
#include "../Resource/Jukebox.hpp"
#include "../Core/Core.h"

class StateMachine;

struct StateData
{
	Jukebox jukebox = Jukebox("resources/Audio/Muisics/");
};

class State
{
public:
	State(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);
	virtual ~State()                    = default;
	State(const State&)					= delete;
	State& operator                     = (const State&)	= delete;

	virtual void onActivate()                  = 0;
	virtual void onDeactivate()                = 0;

	virtual void updateEvents()                = 0;
	virtual void updateImGui()                 = 0;
	virtual void update(const float& dtime)    = 0;
	virtual void draw(sf::RenderTarget* target = nullptr) = 0;

	std::unique_ptr<State> next();

	bool isReplacing() const;
	void updateMousePositions(sf::View* view = nullptr);

    sf::RenderWindow&	window;

protected:

	 bool                   is_replace;
	 StateData&             data;
	 StateMachine&          state_machine;
	 std::unique_ptr<State> next_state;
};
