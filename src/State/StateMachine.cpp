#include "../stdafx.h"
#include "StateMachine.hpp"

StateMachine::StateMachine()
: resume        { false }
, is_running    { false }
, is_restarting { false }
, is_loaded     { false }
, dtime         { 0.0f }
, clock         { }
{
	LOG_INFO("State Machine\t Init");
}

void StateMachine::run(std::unique_ptr<State> state)
{
	is_running    = true;
	is_restarting = false;
	states.push(std::move(state));
}

void StateMachine::nextState()
{
    if (resume)
    {
        // Cleanup the current state
        if (!states.empty())
        {
            states.pop();
        }

        // Resume previous state
        if (!states.empty())
        {
            states.top()->onActivate();
        }

        resume = false;
    }

	// There needs to be a state
	if (!states.empty())
	{
		auto temp = states.top()->next();

		// Only change states if there's a next one existing
		if (temp != nullptr)
		{
			// Replace the running state
			if (temp->isReplacing())
			{
				states.pop();
			}
			// Pause the running state
			else
			{
				states.top()->onDeactivate();
			}

			states.push(std::move(temp));
		}
	}
}

void StateMachine::lastState()
{
    resume = true;
}

void StateMachine::update()
{
	dtime = clock.restart().asMilliseconds();

	while (states.top()->window.pollEvent(core::sfml_event))
	{
		if (core::sfml_event.type == sf::Event::Closed)
		{
			quit();
		}
		if (!states.empty())
		{
			states.top()->updateEvents();
			ImGui::SFML::ProcessEvent(states.top()->window, core::sfml_event);
		}
	}

	if (!states.empty())
	{
		ImGui::SFML::Update(states.top()->window, clock.restart());
		states.top()->updateImGui();
		states.top()->update(dtime);
		data.jukebox.update();
	}
}

void StateMachine::draw()
{
	if (!states.empty())
	{
		states.top()->draw();
	}
}

bool StateMachine::resturting() const
{
	return is_restarting;
}

bool StateMachine::running() const
{
	return is_running;
}

void StateMachine::quit()
{
	is_running = false;
}

void StateMachine::restart(bool restart)
{
	is_restarting = restart;
}