#pragma once
#include "../../Common/Common.h"
#include "../../Animation/Animation.hpp"
#include "../Components/Components.hpp"
#include <Pathfinding/Pathfinding.h>

/**
 * @class Registry
 * @brief A wrapper around an EnTT registry to manage entities and components.
 */
class Registry
{
public:
    Registry();
    virtual ~Registry() = default;

    /// @brief Gets a reference to the internal EnTT registry.
    /// @return Reference to the entt::registry instance.
    entt::registry& getRegistry();

    /// @brief Creates a new entity.
    /// @return The created entity.
    entt::entity createEntity();

    /// @brief Updates all entities in the registry.
    /// @param registry Reference to the EnTT registry.
    /// @param delta_time Time elapsed since the last frame.
    /// @param animator Animator system used for animation updates.
    /// @param pathfinding Pathfinding system for navigation.
    /// @param window SFML render window used for rendering context.
    void update(entt::registry& registry, float delta_time, Animator animator, Pathfinding& pathfinding, sf::RenderWindow& window);

    /// @brief Draws all entities.
    /// @param window The render window to draw to.
    void draw(entt::registry& registry, sf::RenderWindow& window);

private:
    /// @brief Adds a context object(e.g., system or manager) to the registry.
    /// @tparam TContext Type of the context to add.
    /// @param context Context object to store.
    /// @return The added context.
    template <typename TContext>
    TContext addContext(TContext context);

    /// @brief Retrieves a previously stored context object.
    /// @tparam TContext Type of the context to retrieve.
    /// @return Reference to the stored context.
    template <typename TContext>
    TContext& getContext();

private:
    std::unique_ptr <entt::registry> regisry; ///< Internal EnTT registry.
};

/**
 * @class Entity
 * @brief Represents an ECS entity with associated components.
 */
class Entity
{
public:
	Entity(Registry& registry, const std::string& name = "Game object", const std::string& group = "");
	virtual ~Entity() = default;

    /// @brief Destroys the entity from the registry.
    /// @return Always returns 0 (can be extended for error handling).
	std::uint32_t destroy();

    /// @brief Gets the underlying EnTT entity handle.
    /// @return Reference to the internal entt::entity.
	entt::entity& getEntity();

    /// @brief Gets the registry associated with this entity.
    /// @return Reference to the registry managing this entity.
	entt::registry& getRegistry();

    /// @brief Adds a component to an entity.
    /// @tparam Component The type of the component to add.
    /// @tparam Args The types of the arguments to pass to the component constructor.
    /// @param entity The entity to add the component to.
    /// @param args The arguments to pass to the component constructor.
	template <typename TComponent, typename ...Args>
	TComponent& addComponent(Args&& ... args);

    /// @brief Replace a component from an entity.
    /// @tparam Component The type of the component to delete.
    /// @param entity The entity to delete the component from.
	template <typename TComponent, typename ...Args>
	TComponent& replaceComponent(Args&& ... args);

    /// @brief Gets a component from an entity.
    /// @tparam Component The type of the component to get.
    /// @param entity The entity to get the component from.
    /// @return A reference to the component.
	template <typename TComponent, typename ...Args>
	TComponent& getComponent();

    /// @brief Checks if an entity has a specific component.
    /// @tparam Component The type of the component to check for.
    /// @return True if the entity has the component, false otherwise.
	template <typename TComponent>
	bool hasComponent();

    /// @brief Removes a component from the entity.
    /// @tparam TComponent Type of the component to remove.
	template <typename TComponent>
	void removeComponent();

private:
	Registry&      registry; ///< Reference to the registry managing this entity.
    entt::entity   entity;   ///< Handle to the EnTT entity.
    std::string    name;     ///< Name of the entity.
    std::string    group;    ///< Group/category this entity belongs to.

};

#include "Entity.inl"


class UnitFactory 
{
public:
    explicit UnitFactory(Registry& registry, const sf::Texture& texture);

    Entity createSpearman(sf::Vector2f pos);

private:
    Registry&          registry;
    const sf::Texture& unitTexture;
    int                spearmanCounter;
};