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
    PathfindingSystem::  update(registry, pathfinding, delta_time);
    HandleInputSystem::  update(registry, pathfinding);
    MovementSystem::     update(registry, delta_time);
    SpriteUpdateSystem:: update(registry, animator);
    ControlSystem::      update(registry);
    SelectSystem::       update(registry);
}

void EntityManager::draw(sf::RenderWindow& window)
{
    auto view = registry.view<Component_Sprite, Component_Selectable >();
    for (auto entity : view)
    {
        const auto& spriteComponent = view.get<Component_Sprite>(entity);
        window.draw(spriteComponent.sprite);

        const auto& selectComponent = view.get<Component_Selectable>(entity);

        // Отрисовка рамки выделения
        static sf::Vector2f startSelection;
        static bool isSelecting = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!isSelecting) 
            {
                // Устанавливаем стартовую позицию при нажатии
                startSelection = common::mouse_pos_view;
                isSelecting = true;
            }
        }
        else 
{
            if (isSelecting) 
            {
                // Завершаем выделение, когда кнопка отпущена
                isSelecting = false;
            }
        }

        if (isSelecting) {
            sf::RectangleShape selectionBox;
            selectionBox.setFillColor(sf::Color(255, 255, 255, 5)); // Полупрозрачный цвет
            selectionBox.setOutlineColor(sf::Color::Black);
            selectionBox.setOutlineThickness(1);

            // Устанавливаем позицию и размер рамки выделения
            sf::Vector2f currentMousePos = common::mouse_pos_view;
            sf::Vector2f size = currentMousePos - startSelection;

            selectionBox.setPosition(startSelection);
            selectionBox.setSize(size);

            window.draw(selectionBox);
        }
    }

}

void EntityManager::setSprite(entt::entity entity, const std::string& path)
{
    auto& sprite_component = getComponent<Component_Sprite>(entity);

    sprite_component.texture = ResourceLoader::instance().getTexture(path);
    sprite_component.sprite.setTexture(sprite_component.texture);
    sprite_component.int_rect = sf::IntRect(0, 0, sprite_component.texture.getSize().x, sprite_component.texture.getSize().y);
    sprite_component.sprite.setTextureRect(sprite_component.int_rect);
}