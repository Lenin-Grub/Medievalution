#pragma once
#include "State.hpp"

class StateMachine
{
public:
	StateMachine();

	void run(std::unique_ptr<State> state);

	void nextState();
	void lastState();

	void update();
	void draw();

	bool running() const;
	void quit();

	template <typename T>
	static std::unique_ptr<T> build(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	StateData data;

	bool is_loaded;

private:
	float dtime;
	sf::Clock clock;
	std::stack<std::unique_ptr<State>> states;
	bool resume;
	bool is_running;
	bool is_restarting;
};

#include "StateMachine.inl"