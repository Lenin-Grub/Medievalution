#include "stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, m_selected_tile_id (0)
, m_animator_tile_selected_id (0)
, animator(sprite)
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaulatView();
    pathfinding.initNodes(50, 50);
    editor.init();

    texture = ResourceLoader::instance().getTexture("Spearman.png");
    sprite.setTexture(texture);
    animator.setFrameTime(0.5f);
    animator.pause();

    entity_manager.createEntity(sf::Vector2f(0, 300), "rus.png");

    LOG_INFO("State Battle\t Init");
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
        state_machine.lastState();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
        editor.addTile(m_selected_tile_id, common::mouse_pos_view);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !ImGui::GetIO().WantCaptureMouse)
        editor.removeTile(m_selected_tile_id, common::mouse_pos_view);

    if (!ImGui::GetIO().WantCaptureMouse)
        pathfinding.handleInput();

    if (pathfinding.start_node != nullptr && pathfinding.end_node != nullptr)
    {
        sprite.setPosition(pathfinding.start_node->position.x * pathfinding.tile_size.x,
            pathfinding.start_node->position.y * pathfinding.tile_size.y);

    }

    data.camera.scroll();
    data.camera.zoom();
}

void BattleState::updateImGui() 
{
    GUI::updateBattleStateImGui(*this, *this);
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
    pathfinding.move(dtime);

    entity_manager.update(1.0f / 60.0f);

    // Обновляем позицию квадрата
    if (pathfinding.current_node != nullptr && pathfinding.start_node != nullptr && pathfinding.end_node != nullptr)
    {
        sprite.setPosition(pathfinding.current_node->position.x * pathfinding.tile_size.x,
                           pathfinding.current_node->position.y * pathfinding.tile_size.y);
    }
    animator.update(0.1f);
    data.camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;

    target->setView(common::view);

    const auto& layers = editor.getLayers();
    for (const auto& layer : layers)
    {
        if (layer->visible)
            target->draw(*layer);
    }

    pathfinding.draw(window);
    target->draw(sprite);
    entity_manager.draw(window);

    target->setView(window.getDefaultView());
    target->setView(common::view);

    ImGui::SFML::Render(window);
}