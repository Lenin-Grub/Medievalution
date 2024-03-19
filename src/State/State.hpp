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
	virtual ~State() = default;

	State(const State&) = delete;
	State& operator=(const State&) = delete;

	virtual void pause()									= 0;
	virtual void resume()									= 0;

	virtual void updateEvents()								= 0;
	virtual void updateImGui()								= 0;
	virtual void update(const float& dtime)					= 0;
	virtual void draw(sf::RenderTarget* target = nullptr)	= 0;

	std::unique_ptr<State> next();

	bool isReplacing() const;
	void updateMousePositions(sf::View* view = nullptr);

    sf::RenderWindow&	window;
protected:
	StateData&			data;
    StateMachine&		state_machine;

	bool m_replacing;

	std::unique_ptr<State> m_next;
};
