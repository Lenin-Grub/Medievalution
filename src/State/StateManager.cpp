#include "../stdafx.h"
#include "StateManager.h"


StateMachine::StateMachine():
	inserted_state_ID(0)
{
}

StateMachine::~StateMachine()
{
}

int StateMachine::add(std::shared_ptr<State> state)
{
	auto inserted = states.insert(std::make_pair(inserted_state_ID, state));
	LOG_INFO("State added. Count of state {}", states.size());

	return inserted_state_ID++;
}

void StateMachine::end()
{
	auto it = states.find(inserted_state_ID -1);
	if (it != states.end())
	{
		if (current_state == it->second)
		{
			current_state = nullptr;
		}
		states.erase(it);
		LOG_INFO("State ended. Count of state {}", states.size());
	}
}

void StateMachine::switchTo(int id, bool replace)
{
	if (replace)
	{
		end();
	}
	auto it = states.find(id);
	if (it != states.end())
	{
		current_state = it->second;
	}
}

void StateMachine::processEvent()
{
	if (current_state)
	{
		current_state->updateEvents();
	}
}

void StateMachine::updateImGui()
{
	if (current_state)
	{
		current_state->updateImGui();
	}
}

void StateMachine::update(float deltaTime)
{
	if (current_state)
	{
		current_state->update(deltaTime);
	}
}

void StateMachine::draw(sf::RenderTarget& window)
{
	if (current_state)
	{
		current_state->render(&window);
	}
}
