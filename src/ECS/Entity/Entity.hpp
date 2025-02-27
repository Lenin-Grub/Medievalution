#pragma once
#include "../../Common/Common.h"
#include "../../Animation/Animation.hpp"
#include "../Components/Components.hpp"

class EntityManager
{
public:
    EntityManager();
    virtual ~EntityManager();

    bool init();

    entt::entity createEntity();
    void destroyEntity(entt::entity entity);

    void update(float delta_time, Animator animator);
    void draw(sf::RenderWindow& window);

    void setSprite(entt::entity entity, const std::string& path);

    template <typename Component, typename... Args>
    void addComponent(entt::entity entity, Args&&... args);

    template <typename Component>
    void deleteComponent(entt::entity entity);

    template <typename Component>
    Component& getComponent(entt::entity entity);

private:
    template <typename Component>
    bool hasComponent(entt::entity entity);

private:
    entt::registry registry;
};

template <typename Component, typename... Args>
void EntityManager::addComponent(entt::entity entity, Args&&... args)
{
    if (!hasComponent<Component>(entity))
        registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
    LOG_ERROR("Component already has");
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