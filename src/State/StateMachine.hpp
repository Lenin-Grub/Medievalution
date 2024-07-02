#pragma once
#include "State.hpp"

/**
 * @class StateMachine
 * @brief A class to manage and transition between various states in an application.
 *
 * The StateMachine class is designed to handle states within an application,
 * such as a game, settings, loading screan and etc. It allows updating, drawing, and transitioning
 * between different states, and manages the overall state of the application.
 */

class StateMachine
{
public:
	StateMachine   ();

/// @brief Begins running the specified state.
/// @param state The initial state to run.
	void run(std::unique_ptr<State> state);

/// @brief Transitions to the next state in the stack.
	void nextState();

/// @brief Reverts to the previous state in the stack.
	void lastState();

/// @brief Updates the current state.
/// Calculates the time delta and calls the update function of the state on top of the stack.
	void update();

/// @brief Draws the current state.
/// Calls the draw function of the state on top of the stack.
	void draw();

/// @brief Checks if the state machine is restarting.
/// @return true if the state machine is restarting, false otherwise.
	bool resturting() const;

/// @brief Checks if the state machine is currently running.
/// @return true if the state machine is running, false otherwise.
	bool running() const;

/// @brief Stops the state machine.
	void quit();

/// @brief Restarts the state machine.
/// @param restart Indicates whether to restart the state machine(default is false).
	void restart(bool restart = false);

/// @brief Creates and initializes a new state. Called first.
	template <typename T>
	static std::unique_ptr<T> build(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	StateData data; ///< @brief Holds information necessary for the states to function.
	bool is_loaded; ///< @brief Indicates whether the state machine is loaded and ready.

private:
	float                              dtime;         ///< @brief Time delta between updates.
	sf::Clock                          clock;         ///< @brief Clock to manage time-related operations.
	std::stack<std::unique_ptr<State>> states;        ///< @brief Stack holding the states, with the current state on top.
	bool                               resume;        ///< @brief Indicates whether the state machine should resume from the previous state.
	bool                               is_running;    ///< @brief Indicates whether the state machine is currently running.
	bool                               is_restarting; ///< @brief Indicates whether the state machine is in the process of restarting.
};

#include "StateMachine.inl"