#pragma once
#include "State.hpp"

class StateMachine
{
public:
	StateMachine   ();

	void run(std::unique_ptr<State> state);
	void nextState ();
	void lastState ();
	void update    ();
	void draw      ();
	bool resturting() const;
	bool running   () const;
	void quit      ();
	void restart(bool restart = false);

	template <typename T>
	static std::unique_ptr<T> build(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	bool      is_loaded;
	StateData data;

private:
	float                              dtime;
	bool                               resume;
	bool                               is_running;
	bool                               is_restarting;
	sf::Clock                          clock;
	std::stack<std::unique_ptr<State>> states;
};

#include "StateMachine.inl"