#include "stdafx.h"
#include "Entity.hpp"

#include "../../Resource/ResourceManager.hpp"
#include "../Systems/ControlSystem/ControlSystem.hpp"
#include "../Systems/HandleInputSystem/HandleInputSystem.hpp"
#include "../Systems/MovementSystem/MovementSystem.hpp"
#include "../Systems/SpriteUpdateSystem/SpriteUpdateSystem.hpp"
#include "../Systems/PathfindingSystem/PathfindingSystem.hpp"
#include "../Systems/SelectSystem/SelectSystem.hpp"


EntityManager::EntityManager() 
{
}

EntityManager::~EntityManager()
{
}

bool EntityManager::init()
{
    return false;
}


entt::entity EntityManager::createEntity()
{
    auto entity = registry.create();
    return entity;
}

void EntityManager::destroyEntity(entt::entity entity)
{
    registry.destroy(entity);
}

void EntityManager::update(float delta_time, Animator animator, Pathfinding& pathfinding, sf::RenderWindow& window)
{
    SpriteUpdateSystem:: update(registry, animator);
    HandleInputSystem::  update(registry, pathfinding);
    PathfindingSystem::  update(registry, pathfinding, delta_time);
    MovementSystem::     update(registry, delta_time);
    ControlSystem::      update(registry);
    SelectSystem::       update(registry);
}

void EntityManager::draw(sf::RenderWindow& window)
{
    auto view = registry.view<Components::Sprite>();
    for (auto entity : view)
    {
        const auto& spriteComponent = view.get<Components::Sprite>(entity);

        window.draw(spriteComponent.sprite);
    }
    
    selectionBox(window);
}

void EntityManager::selectionBox(sf::RenderWindow& window)
{
    // Отрисовка рамки выделения
    static sf::Vector2f startSelection;
    static bool isSelecting = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!isSelecting)
        {
            startSelection = common::mouse_pos_view;
            isSelecting = true;
        }
    }
    else
    {
        if (isSelecting)
            isSelecting = false;
    }

    if (isSelecting)
    {
        sf::RectangleShape selectionBox;
        selectionBox.setFillColor(sf::Color(255, 255, 255, 5));
        selectionBox.setOutlineColor(sf::Color::Black);
        selectionBox.setOutlineThickness(1);

        sf::Vector2f currentMousePos = common::mouse_pos_view;
        sf::Vector2f size = currentMousePos - startSelection;

        selectionBox.setPosition(startSelection);
        selectionBox.setSize(size);

        window.draw(selectionBox);
    }
}

void EntityManager::setSprite(entt::entity entity, const std::string& path)
{
    auto& sprite_component = getComponent<Components::Sprite>(entity);

    sprite_component.texture = ResourceLoader::instance().getTexture(path);
    sprite_component.sprite.setTexture(sprite_component.texture);
    sprite_component.int_rect = sf::IntRect(0, 0, sprite_component.texture.getSize().x, sprite_component.texture.getSize().y);
    sprite_component.sprite.setTextureRect(sprite_component.int_rect);
}