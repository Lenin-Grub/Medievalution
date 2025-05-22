#include "stdafx.h"
#include "Entity.hpp"

#include "../../Resource/ResourceManager.hpp"
#include "../Systems/AllSystems.hpp"


#pragma region Registry
    Registry::Registry()
        :regisry(nullptr)
    {
        regisry = std::make_unique<entt::registry>();
    }

    entt::registry& Registry::getRegistry()
    {
        if (!regisry)
            LOG_ERROR("Registry is not initialized");

        return *regisry;
    }

    entt::entity Registry::createEntity()
    {
        return regisry->create();
    }

    void Registry::update(entt::registry& registry, float delta_time, Animator animator, Pathfinding& pathfinding, sf::RenderWindow& window)
    {
        SpriteSystem::      update(registry, animator);
        HandleInputSystem:: update(registry, pathfinding);
        PathfindingSystem:: update(registry, pathfinding, delta_time);
        MovementSystem::    update(registry, delta_time);
        ControlSystem::     update(registry);
        SelectSystem::      update(registry);
        AnimationSystem::   update(registry, delta_time);
        StateSystem::       update(registry, delta_time);
    }

    void Registry::draw(entt::registry& registry, sf::RenderWindow& window)
    {
        RenderSystem::render(registry, window);
    }

#pragma endregion



#pragma region Entity
    Entity::Entity(Registry& registry, const std::string& name, const std::string& group)
        : registry(registry)
        , name(name)
        , group(group)
    {
        entity = registry.createEntity();
        addComponent<Components::Identification>(Components::Identification
            {
                .name = name,
                .group = group,
                .id = static_cast<int32_t>(entity)
            });
    }

    std::uint32_t Entity::destroy()
    {
        return registry.getRegistry().destroy(entity);
    }

    entt::entity& Entity::getEntity()
    {
        return entity;
    }

    entt::registry& Entity::getRegistry()
    {
        return registry.getRegistry();
    }
#pragma endregion