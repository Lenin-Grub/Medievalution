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

    // Function to create entities with random positions and velocities
void createEntities(EntityManager& entity_manager, sf::Sprite defaultSprite)
{
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
        entity_manager.addComponent<Component_Sprite>(entity, defaultSprite);

        // Set a random sprite for demonstration; you can customize this part
        entity_manager.setSprite(entity, "path/to/your/texture" + std::to_string(i % 2) + ".png"); // Example path
    }
}

// Update function to ensure entities stay within bounds
void updateEntities(EntityManager& entity_manager, float deltaTime)
{
    auto view = entity_manager.registry.view<Component_Position, Component_Velocity>();
    for (auto entity : view)
    {
        auto& position = view.get<Component_Position>(entity);
        auto& velocity = view.get<Component_Velocity>(entity);

        // Update position
        position.position += velocity.velocity * deltaTime;

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
private:
    template <typename Component>
    bool hasComponent(entt::entity entity);

private:
    entt::registry registry;
};

template <typename Component, typename... Args>
void EntityManager::addComponent(entt::entity entity, Args&&... args)
{
    registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
}

template <typename Component>
void EntityManager::deleteComponent(entt::entity entity)
{
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