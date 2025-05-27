#include  "MovementSystem.hpp"

void MovementSystem::update(entt::registry& registry, float delta_time)
{
    auto view = registry.view<Components::Position, Components::Velocity>(entt::exclude<Components::Pathfinding>);
    for (auto [entity, position, velocity] : view.each())
    {
        position.position += velocity.velocity * delta_time;
    }
}