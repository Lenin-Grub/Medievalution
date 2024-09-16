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

	if (Input::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		id++;
	}
	if (Input::isKeyPressed(sf::Keyboard::Key::Num2))
	{
		id--;
	}


	if (Input::isMouseReleased(sf::Mouse::Left))
	{
		sf::Vector2u tilePos = sf::Vector2u(core::mouse_pos_view.x / 64, core::mouse_pos_view.y / 64);
		battleground.update(tilePos.x, tilePos.y, id);
		
	}

	//pathfinding.handleInput();
	camera.scroll();
	camera.zoom();
}

void BattleState::updateImGui()
{
	ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button((char*)ICON_LOG_IN "Выход###toMainMenu", ImVec2(120, 0)))
	{
		state_machine.lastState();
	}
	ImGui::End();

	ImGui::Begin("Editor", nullptr);

	std::string message = "World editor";
	ImGui::Text(message.data(), message.data() + message.size());
	ImGui::Text("Tile id: %d", id);

	ImGui::Image(battleground.m_tileset);

	ImGui::End();
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
