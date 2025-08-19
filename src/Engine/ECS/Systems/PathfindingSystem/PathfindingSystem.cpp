#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time)
{
    global_pathfinding.resetWalkable();

    auto all_units = registry.view<Components::Position>();
    for (auto entity : all_units)
    {
        auto& pos = registry.get<Components::Position>(entity);
        Node* node = global_pathfinding.getNodeByPosition(pos.position);
        if (node)
        {
            node->walkable = false;
        }
    }

    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view)
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);
        auto& vel_comp = registry.get<Components::Velocity>(entity);

        if (path_comp.path.empty() || path_comp.current_node_index >= path_comp.path.size())
        {
            if (path_comp.end_node)
            {
                Node* start_node = global_pathfinding.getNodeByPosition(pos_comp.position);
                if (start_node && start_node != path_comp.end_node)
                {
                    bool was_walkable = start_node->walkable;
                    start_node->walkable = true;

                    bool end_was_walkable = path_comp.end_node->walkable;
                    path_comp.end_node->walkable = true;

                    global_pathfinding.findPath(start_node, path_comp.end_node);
                    path_comp.path = global_pathfinding.path();
                    path_comp.current_node_index = 0;

                    start_node->walkable = was_walkable;
                    path_comp.end_node->walkable = end_was_walkable;

                    if (path_comp.path.empty())
                    {
                        path_comp.end_node = nullptr;
                        setIdleStateIfMoving(registry, entity);
                        continue;
                    }
                }
            }
            else
            {
                setIdleStateIfMoving(registry, entity);
            }
            continue;
        }

        Node* next_node = path_comp.path[path_comp.current_node_index];
        sf::Vector2f direction = next_node->position - pos_comp.position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= vel_comp.speed * delta_time)
        {
            reachNextNode(registry, entity, global_pathfinding);
        }
        else
        {
            moveTowardsNextNode(registry, entity, direction, distance, delta_time);
            setCharacterState(registry, entity);
        }
    }
}

void PathfindingSystem::setIdleStateIfMoving(entt::registry& registry, entt::entity entity)
{
    if (auto* state = registry.try_get<Components::State>(entity))
    {
        if (state->state == Components::CharacterState::Run)
        {
            state->state = Components::CharacterState::Idle;
        }
    }
}

void PathfindingSystem::reachNextNode(entt::registry& registry, entt::entity entity, Pathfinding& global_pathfinding)
{
    auto& path_comp = registry.get<Components::Pathfinding>(entity);
    auto& pos_comp = registry.get<Components::Position>(entity);

    Node* next_node = path_comp.path[path_comp.current_node_index];
    pos_comp.position = next_node->position;

    if (path_comp.current_node_index >= path_comp.path.size() - 1)
    {
        path_comp.start_node = next_node;

        bool target_reached = false;
        if (path_comp.end_node)
        {
            const float epsilon = 5.0f;
            float dx = std::abs(pos_comp.position.x - path_comp.end_node->position.x);
            float dy = std::abs(pos_comp.position.y - path_comp.end_node->position.y);

            if (dx <= epsilon && dy <= epsilon)
            {
                target_reached = true;
            }
        }

        if (target_reached || !path_comp.end_node)
        {
            path_comp.path.clear();
            path_comp.current_node_index = 0;
            setIdleStateIfMoving(registry, entity);
        }
        else
        {
            global_pathfinding.findPath(path_comp.start_node, path_comp.end_node);
            path_comp.path = global_pathfinding.path();
            path_comp.current_node_index = 0;

            if (!path_comp.path.empty())
            {
                setCharacterState(registry, entity);
            }
            else
            {
                LOG_WARN("No path found to target, setting IDLE");
                setIdleStateIfMoving(registry, entity);
            }
        }
    }
    else
    {
        path_comp.current_node_index++;
        setCharacterState(registry, entity);
    }
}

void PathfindingSystem::moveTowardsNextNode(entt::registry& registry, entt::entity entity, const sf::Vector2f& direction, float distance, float delta_time)
{
    auto& pos_comp = registry.get<Components::Position>(entity);
    auto& vel_comp = registry.get<Components::Velocity>(entity);

    if (distance > 0.0f)
    {
        sf::Vector2f normalizedDirection = direction / distance;
        pos_comp.position += normalizedDirection * vel_comp.speed * delta_time;
    }
}

void PathfindingSystem::setCharacterState(entt::registry& registry, entt::entity entity)
{
    if (auto* state = registry.try_get<Components::State>(entity))
    {
        if (state->state != Components::CharacterState::Run)
        {
            state->state = Components::CharacterState::Run;
        }
    }
}

std::vector<Node*> PathfindingSystem::findNearbyEmptyNodes(Pathfinding& pathfinding, const sf::Vector2f& target_position, size_t count)
{
    std::vector<Node*> result;
    if (count == 0) return result;

    Node* center_node = pathfinding.getNodeByPosition(target_position);
    if (!center_node) return result;

    std::vector<std::pair<float, Node*>> nodes_with_distance;

    for (const auto& pair : pathfinding.getNodes())
    {
        Node* node = const_cast<Node*>(&pair.second);

        if (node->walkable)
        {
            sf::Vector2f delta = node->position - target_position;
            float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            nodes_with_distance.push_back({ distance, node });
        }
    }

    std::sort(nodes_with_distance.begin(), nodes_with_distance.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    for (size_t i = 0; i < nodes_with_distance.size() && result.size() < count; ++i)
    {
        result.push_back(nodes_with_distance[i].second);
    }

    return result;
}

void PathfindingSystem::assignFormationPositions(entt::registry& registry, Pathfinding& global_pathfinding, const std::vector<entt::entity>& selected_units, const sf::Vector2f& target_position)
{
    if (selected_units.empty()) return;

    std::set<Node*> occupied_nodes;
    for (auto entity : selected_units)
    {
        auto& pos_comp = registry.get<Components::Position>(entity);
        Node* current_node = global_pathfinding.getNodeByPosition(pos_comp.position);
        if (current_node)
        {
            occupied_nodes.insert(current_node);
        }
    }

    auto target_nodes = findNearbyEmptyNodes(global_pathfinding, target_position, selected_units.size());

    for (size_t i = 0; i < selected_units.size() && i < target_nodes.size(); ++i)
    {
        auto entity = selected_units[i];
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);

        Node* target_node = target_nodes[i];
        Node* start_node = global_pathfinding.getNodeByPosition(pos_comp.position);

        if (start_node && target_node && start_node != target_node)
        {
            path_comp.end_node = target_node;
            path_comp.start_node = start_node;

            bool target_was_walkable = target_node->walkable;
            target_node->walkable = true;

            std::vector<std::pair<Node*, bool>> temporarily_made_walkable;
            for (Node* occupied_node : occupied_nodes)
            {
                if (occupied_node != start_node)
                {
                    bool was_walkable = occupied_node->walkable;
                    occupied_node->walkable = true;
                    temporarily_made_walkable.push_back({ occupied_node, was_walkable });
                }
            }

            global_pathfinding.findPath(path_comp.start_node, path_comp.end_node);
            path_comp.path = global_pathfinding.path();
            path_comp.current_node_index = 0;

            target_node->walkable = target_was_walkable;
            for (const auto& [node, original_state] : temporarily_made_walkable)
            {
                node->walkable = original_state;
            }
        }
    }
}