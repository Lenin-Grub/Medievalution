#pragma once
#include "../../Common/Common.h"
#include "../../Animation/Animation.hpp"
#include "../Components/Components.hpp"
#include <Pathfinding/Pathfinding.h>


/// @class EntityManager
/// @brief Manages entities and their components in the game.
class EntityManager
{
public:
    /// @brief Constructs a new EntityManager object.
    EntityManager();

    /// @brief Destroys the EntityManager object.
    virtual ~EntityManager();

    /// @brief Initializes the EntityManager.
    /// @return True if initialization is successful, false otherwise.
    bool init();

    /// @brief Creates a new entity.
    /// @return The created entity.
    entt::entity createEntity();

    /// @brief Destroys an existing entity.
    /// @param entity The entity to destroy
    void destroyEntity(entt::entity entity);

    /// @brief Updates all entities.
    /// @param delta_time The time elapsed since the last update.
    /// @param animator The animator used for updating animations.
    void update(float delta_time, Animator animator, Pathfinding& pathfinding, sf::RenderWindow& window);

    /// @brief Draws all entities.
    /// @param window The render window to draw to.
    void draw(sf::RenderWindow& window);

    void selectionBox(sf::RenderWindow& window);

    /// @brief Sets the sprite for an entity.
    /// @param entity The entity to set the sprite for.
    /// @param path The file path to the sprite image.
    void setSprite(entt::entity entity, const std::string& path);

    /// @brief Adds a component to an entity.
    /// @tparam Component The type of the component to add.
    /// @tparam Args The types of the arguments to pass to the component constructor.
    /// @param entity The entity to add the component to.
    /// @param args The arguments to pass to the component constructor.
    template <typename Component, typename... Args>
    void addComponent(entt::entity entity, Args&&... args);

    /// @brief Deletes a component from an entity.
    /// @tparam Component The type of the component to delete.
    /// @param entity The entity to delete the component from.
    template <typename Component>
    void deleteComponent(entt::entity entity);

    /// @brief Gets a component from an entity.
    /// @tparam Component The type of the component to get.
    /// @param entity The entity to get the component from.
    /// @return A reference to the component.
    template <typename Component>
    Component& getComponent(entt::entity entity);

    entt::registry registry;
private:
    /// @brief Checks if an entity has a specific component.
    /// @tparam Component The type of the component to check for.
    /// @param entity The entity to check.
    /// @return True if the entity has the component, false otherwise.
    template <typename Component>
    bool hasComponent(entt::entity entity);


private:
};

#include "Entity.inl"