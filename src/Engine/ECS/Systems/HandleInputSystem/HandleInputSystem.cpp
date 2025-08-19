#include "HandleInputSystem.hpp"
#include <ECS/Systems/FormationSystem/FormationSystem.hpp>
#include <ECS/Systems/PathfindingSystem/PathfindingSystem.hpp>


void HandleInputSystem::update(entt::registry& registry, Pathfinding& global_pathfinding)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) 
    {

        std::vector<entt::entity> selected_units;
        auto view = registry.view<Components::Pathfinding, Components::Position, Components::Selectable>();

        for (auto entity : view) 
        {
            auto& selectable = view.get<Components::Selectable>(entity);
            if (selectable.is_selected) 
            {
                selected_units.push_back(entity);
            }
        }

        if (!selected_units.empty()) 
        {
            Node* target_node = global_pathfinding.getNodeByMousePosition(common::mouse_pos_view);
            if (target_node) 
            {
                std::vector<entt::entity> squad_units;
                std::vector<entt::entity> regular_units;

                for (auto entity : selected_units)
                {
                    if (registry.all_of<Components::Formation>(entity))
                    {
                        squad_units.push_back(entity);
                    }
                    else 
                    {
                        regular_units.push_back(entity);
                    }
                }

                if (!regular_units.empty()) 
                {
                    assignFormationPositions(registry, global_pathfinding, regular_units, target_node->position);
                }

                if (!squad_units.empty()) 
                {
                    bool has_squad_leader = false;
                    std::vector<uint32_t> selected_squad_ids;


                    std::sort(selected_squad_ids.begin(), selected_squad_ids.end());
                    selected_squad_ids.erase(std::unique(selected_squad_ids.begin(), selected_squad_ids.end()), selected_squad_ids.end() );

                    if (has_squad_leader && selected_squad_ids.size() == 1) 
                    {
                        auto all_squad_members = SquadMovementSystem::getSquadMembers(registry, selected_squad_ids[0]);
                        bool all_members_selected = true;

                        for (auto member : all_squad_members) 
                        {
                            if (auto* selectable = registry.try_get<Components::Selectable>(member)) 
                            {
                                if (!selectable->is_selected) 
                                {
                                    all_members_selected = false;
                                    break;
                                }
                            }
                            else 
                            {
                                all_members_selected = false;
                                break;
                            }
                        }

                        if (all_members_selected) 
                        {
                            for (auto entity : squad_units)
                            {
                                moveSquadUnit(registry, global_pathfinding, entity, target_node);
                            }
                        }
                        else 
                        {
                            assignFormationPositions(registry, global_pathfinding, squad_units, target_node->position);
                        }
                    }
                    else 
                    {
                        assignFormationPositions(registry, global_pathfinding, squad_units, target_node->position);
                    }
                }
            }
        }
    }
}

void HandleInputSystem::assignFormationPositions(entt::registry& registry, Pathfinding& global_pathfinding,
    const std::vector<entt::entity>& selected_units,
    const sf::Vector2f& target_position)
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

    auto target_nodes = PathfindingSystem::findNearbyEmptyNodes(global_pathfinding, target_position, selected_units.size());

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

void HandleInputSystem::moveSquadUnit(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity entity, Node* target_node) 
{
    auto& path_component = registry.get<Components::Pathfinding>(entity);
    auto& position_component = registry.get<Components::Position>(entity);

    path_component.end_node = target_node;
    path_component.start_node = global_pathfinding.getNodeByPosition(position_component.position);

    if (path_component.start_node && path_component.end_node) 
    {
        bool target_was_walkable = target_node->walkable;
        target_node->walkable = true;

        global_pathfinding.findPath(path_component.start_node, path_component.end_node);
        path_component.path = global_pathfinding.path();
        path_component.current_node_index = 0;

        target_node->walkable = target_was_walkable;

        if (path_component.path.empty()) 
        {
            path_component.end_node = nullptr;
            if (auto* state = registry.try_get<Components::State>(entity)) 
            {
                if (state->state == Components::CharacterState::Run) 
                {
                    state->state = Components::CharacterState::Idle;
                }
            }
        }
    }
}