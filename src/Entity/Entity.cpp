#include "stdafx.h"
#include "Entity.hpp"

#include "../Resource/ResourceManager.hpp"


EntityManager::EntityManager() 
{
}

void EntityManager::createEntity(sf::Vector2f pos, const sf::Sprite& sprite)
{
    auto entity = registry.create();
    registry.emplace<Component_Position>(entity, pos);
    registry.emplace<Component_Velocity>(entity, sf::Vector2f(0.0f, 0.0f));
    registry.emplace<Control>(entity);

    registry.emplace<Component_Sprite>(entity, sprite);
}

void EntityManager::destroyEntity(entt::entity entity) 
{
    registry.destroy(entity);
}

void EntityManager::update(float delta_time, Animator animator)
{
    HandleInputSystem::handleInput(registry);
    ControlSystem::controlSystem(registry);
    MovementSystem::update(registry, delta_time);
    SpriteUpdateSystem::update(registry, animator);
}

void EntityManager::draw(sf::RenderWindow& window) 
{
    auto view = registry.view<Component_Sprite>();
    for (auto entity : view) 
    {
        const auto& sprite = view.get<Component_Sprite>(entity);
        window.draw(sprite.sprite);
    }
}


void MovementSystem::update(entt::registry& registry, float deltaTime) 
{
    auto view = registry.view<Component_Position, Component_Velocity>();
    for (auto entity : view) 
    {
        auto& position       = view.get<Component_Position>(entity);
        const auto& velocity = view.get<Component_Velocity>(entity);

        position.position += velocity.velocity * deltaTime;
    }
}

void SpriteUpdateSystem::update(entt::registry& registry, Animator animator)
{
    auto spriteView = registry.view<Component_Position, Component_Sprite>();
    for (auto entity : spriteView) 
    {
        auto& sprite         = spriteView.get<Component_Sprite>(entity);
        const auto& position = spriteView.get<Component_Position>(entity);

        sprite.sprite.setPosition(position.position);
        sprite.sprite.setTextureRect(animator.getCurrentFrameRect());
    }
}

void HandleInputSystem::handleInput(entt::registry& registry)
{
    auto view = registry.view<Control>();
    for (auto entity : view) 
    {
        auto&         control = view.get<Control>(entity);
        control.up            = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        control.down          = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        control.left          = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        control.right         = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    }
}

void ControlSystem::controlSystem(entt::registry& registry)
{
    auto view = registry.view<Component_Velocity, Control>();
    for (auto entity : view) 
    {
        auto&       velocity = view.get<Component_Velocity>(entity);
        const auto& control  = view.get<Control>(entity);

        float vel = 50.0f;

        velocity.velocity.x = (control.right ? vel : 0.0f) - (control.left ? vel : 0.0f);
        velocity.velocity.y = (control.down  ? vel : 0.0f) - (control.up   ? vel : 0.0f);
    }
}
