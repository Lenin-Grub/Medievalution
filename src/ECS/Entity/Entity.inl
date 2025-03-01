#pragma once
#include "Entity.hpp"

template <typename Component, typename... Args>
void EntityManager::addComponent(entt::entity entity, Args&&... args)
{
    if (!hasComponent<Component>(entity))
        registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
    else
    {
        LOG_ERROR("Component already has");
        return;
    }
}

template <typename Component>
void EntityManager::deleteComponent(entt::entity entity)
{
    if (hasComponent<Component>(entity))
        registry.remove<Component>(entity);
}

template <typename Component>
Component& EntityManager::getComponent(entt::entity entity)
{
    return registry.get<Component>(entity);
}

template <typename Component>
bool EntityManager::hasComponent(entt::entity entity)
{
    return registry.all_of<Component>(entity);
}