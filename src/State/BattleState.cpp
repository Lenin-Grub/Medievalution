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
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    animator.setFrameTime(0.5f);
    animator.pause();

    auto entity = entity_manager.createEntity();
    entity_manager.addComponent<Component_Position>(entity, sf::Vector2f(0.0f, 0.0f));
    entity_manager.addComponent<Component_Velocity>(entity, sf::Vector2f(0.0f, 0.0f));
    entity_manager.addComponent<Component_Sprite>(entity, sprite);
    entity_manager.addComponent<Control>(entity);

    entity_manager.setSprite(entity, "Spearman.png");

    auto entity2 = entity_manager.createEntity();
    entity_manager.addComponent<Component_Position>(entity2, sf::Vector2f(300.0f, 300.0f));
    entity_manager.addComponent<Component_Sprite>(entity2, sprite);

    entity_manager.setSprite(entity2, "Archer.png");
    ///-------------

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(0.0f, 1000.0f);
    std::uniform_real_distribution<float> velDist(-50.0f, 50.0f); // Adjust velocity range as needed

    for (int i = 0; i < 10; ++i)
    {
        auto entity = entity_manager.createEntity();
        sf::Vector2f randomPosition(posDist(gen), posDist(gen));
        sf::Vector2f randomVelocity(velDist(gen), velDist(gen));

        entity_manager.addComponent<Component_Position>(entity, randomPosition);
        entity_manager.addComponent<Component_Velocity>(entity, randomVelocity);
        entity_manager.addComponent<Component_Sprite>(entity, sprite);
        entity_manager.setSprite(entity, "Archer.png");
    }

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

    entity_manager.update(1.0f / 60.0f, animator);


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