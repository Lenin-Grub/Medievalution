#include "stdafx.h"
#include "Entity.hpp"

#include "../../Resource/ResourceManager.hpp"
#include "../Systems/ControlSystem/ControlSystem.hpp"
#include "../Systems/HandleInputSystem/HandleInputSystem.hpp"
#include "../Systems/MovementSystem/MovementSystem.hpp"
#include "../Systems/SpriteUpdateSystem/SpriteUpdateSystem.hpp"


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

void EntityManager::update(float delta_time, Animator animator)
{
    HandleInputSystem::update(registry);
    ControlSystem::update(registry);
    MovementSystem::update(registry, delta_time);
    SpriteUpdateSystem::update(registry, animator);


    auto view = registry.view<Component_Position, Component_Velocity>();
    for (auto entity : view)
    {
        auto& position = view.get<Component_Position>(entity);
        auto& velocity = view.get<Component_Velocity>(entity);

        // Check boundaries and reverse direction if needed
        if (position.position.x <= 0 || position.position.x >= 1000)
        {
            velocity.velocity.x = -velocity.velocity.x;
            position.position.x = std::clamp(position.position.x, 0.0f, 1000.0f);
        }
        if (position.position.y <= 0 || position.position.y >= 1000)
        {
            velocity.velocity.y = -velocity.velocity.y;
            position.position.y = std::clamp(position.position.y, 0.0f, 1000.0f);
        }
    }
}

void EntityManager::draw(sf::RenderWindow& window)
{
    auto view = registry.view<Component_Sprite>();
    for (auto entity : view)
    {
        const auto& spriteComponent = view.get<Component_Sprite>(entity);
        window.draw(spriteComponent.sprite);
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