#include "Entity.hpp"

#include <Resource/ResourceManager.hpp>
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

    void Registry::update(entt::registry& registry, float delta_time, Pathfinding& pathfinding, sf::RenderWindow& window)
    {
        PathfindingSystem::update(registry, pathfinding, delta_time);
        ControlSystem    ::update(registry);
        MovementSystem   ::update(registry, delta_time);
        SpriteSystem     ::update(registry);
        SelectSystem     ::update(registry);
        AnimationSystem  ::update(registry, delta_time);
        StateSystem      ::update(registry, delta_time);
        HandleInputSystem::update(registry, pathfinding);
        FormationSystem  ::update(registry, pathfinding);
    }

    void Registry::draw(entt::registry& registry, sf::RenderTarget& window)
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


    // for tests only
    UnitFactory::UnitFactory(Registry& registry, const sf::Texture& texture)
        : registry(registry), unitTexture(texture), spearmanCounter(0) 
    {
    }

    Entity UnitFactory::createSpearman(sf::Vector2f pos)
    {
        std::ostringstream oss;
        oss << "Swordman " << ++spearmanCounter;
        std::string name = oss.str();

        Entity entity(registry, name, "Characters");

        entity.addComponent<Components::Position>(Components::Position{
            .position = pos
            });

        entity.addComponent<Components::Velocity>(Components::Velocity{
            .velocity = sf::Vector2f(0.0f, 0.0f),
            .speed = 70.0f
            });

        entity.addComponent<Components::Sprite>(Components::Sprite{
            .sprite = sf::Sprite(unitTexture)
            });

        auto& spriteComponent = entity.getComponent<Components::Sprite>();
        spriteComponent.sprite.setOrigin(8.0f, 32.0f);
        spriteComponent.sprite.setTextureRect(sf::IntRect(0, 0, 44, 44));

        entity.addComponent<Components::Pathfinding>(Components::Pathfinding{});
        entity.addComponent<Components::Selectable>(Components::Selectable{});
        entity.addComponent<Components::Animation>(Components::Animation{ spriteComponent.sprite });
        //entity.addComponent<Components::Squad>(Components::Squad{});

        auto& animator = entity.getComponent<Components::Animation>().animator;
        AnimationLoader::loadFromFile("Swordman", animator);

        entity.addComponent<Components::State>(Components::State{
            .state = Components::CharacterState::Idle
            });

        return entity;
    }
