#include "stdafx.h"
#include "Entity.hpp"

#include "../Resource/ResourceManager.hpp"

EntityManager::EntityManager() 
{
}

void EntityManager::createEntity(sf::Vector2f pos, const std::string& texture_path)
{
    auto entity = registry.create();
    registry.emplace<Component_Position>(entity, pos);
    registry.emplace<Component_Velocity>(entity, sf::Vector2f(10.0f, 0.0f));

    sf::Sprite sprite;
    texture = ResourceLoader::instance().getTexture(texture_path);
    sprite.setTexture(texture);
    registry.emplace<Component_Sprite>(entity, sprite);
}

void EntityManager::destroyEntity(entt::entity entity) 
{
    registry.destroy(entity);
}

void EntityManager::update(float deltaTime) 
{
    MovementSystem::update(registry, deltaTime);
    SpriteUpdateSystem::update(registry);
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

        position.position.x += velocity.velocity.x * deltaTime;
        position.position.y += velocity.velocity.y * deltaTime;
    }
}

void SpriteUpdateSystem::update(entt::registry& registry) 
{
    auto spriteView = registry.view<Component_Position, Component_Sprite>();
    for (auto entity : spriteView) 
    {
        auto& sprite         = spriteView.get<Component_Sprite>(entity);
        const auto& position = spriteView.get<Component_Position>(entity);

        sprite.sprite.setPosition(position.position);
    }
}