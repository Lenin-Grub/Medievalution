#include  "MovementSystem.hpp"
#include <numbers>

void MovementSystem::update(entt::registry& registry, float delta_time)
{
    //auto view = registry.view<Components::Position, Components::Velocity, Components::Direction>();

    //for (auto entity : view)
    //{
    //    auto& position = view.get<Components::Position>(entity);
    //    auto& velocity = view.get<Components::Velocity>(entity);
    //    auto& control  = view.get<Components::Direction>(entity);

    //    sf::Vector2f direction = Math::normalizeVector(control.direction);

    //    position.position += Math::multiply(direction, velocity.speed * delta_time);

    //    if (direction.x != 0.0f || direction.y != 0.0f)
    //        position.angle = std::atan2(direction.y, direction.x) * 180.0f / std::numbers::pi;
    //}

    auto view = registry.view<Components::Position, Components::Velocity>(entt::exclude<Components::Pathfinding>);
    for (auto [entity, position, velocity] : view.each())
    {
        position.position += velocity.velocity * delta_time;
    }
}