#include "../stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, camera()
, battleground()
{
	LOG_INFO("State Battle\t Init");
	pathfinding.initNodes(100, 100);
	    const int level[] =
    {
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 16, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 16, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
    };
	battleground.load("resources/Map/tileset.png", sf::Vector2u(64, 64), level, 16, 16);
}

void BattleState::onDeactivate()
{
	LOG_INFO("State Battle\t Deactivate");
}

void BattleState::onActivate()
{
	LOG_INFO("State Battle\t Activate"); 
}

void BattleState::updateEvents()
{
	if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		state_machine.lastState();
	}

	//pathfinding.handleInput();
	camera.scroll();
	camera.zoom();
}

void BattleState::updateImGui()
{
}

void BattleState::update(const float& dtime)
{
	updateMousePositions();
	//pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
	camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
	window.clear();
	if (!target)
		target = &window;
	target->setView(core::view);

	//pathfinding.draw(window);
    target->draw(battleground);

	target->setView(window.getDefaultView());
	target->setView(core::view);

	ImGui::SFML::Render(window);

	window.display();
}
