#include "stdafx.h"
#include "Entity.hpp"

#include "../../Resource/ResourceManager.hpp"
#include "../Systems/AllSystems.hpp"


entt::entity EntityManager::createEntity(const std::string& name, const std::string& group)
{
    auto   entity = registry.create();
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
    RenderSystem::render(registry, window);    
}

const entt::registry& EntityManager::getRegistry()
{
    return registry;
}

bool EntityManager::hasEntity(entt::entity entity) const
{
    return registry.valid(entity);
}