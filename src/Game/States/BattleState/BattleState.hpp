#pragma once
#include "../State/State.hpp"

#include <Animation/Animator/Animator.hpp>
#include <Animation/AnimationEditor/AnimationEditor.hpp>
#include <Pathfinding/Pathfinding.h>
#include <ECS/Entity/Entity.hpp>
#include <ECS/Systems/PathfindingSystem/PathfindingSystem.hpp>
#include <Gizmos/Gizmos.h>

#include <World/BattleMap/BattleMap.hpp>

class StateMachine;

class BattleState final 
    : public State
{
public:
    BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

    void init()                                   override;
    void onDeactivate()                           override;
    void onActivate  ()                           override;
    void updateEvents()                           override;
    void updateImGui ()                           override;
    void update(const float& dtime)               override;
    void draw(sf::RenderTarget* target = nullptr) override;

private:
    void endView(sf::RenderTarget* target);
    void beginView(sf::RenderTarget*& target);

    std::vector<uint32_t> getSelectedSquads();

    void changeSquadFormation(uint32_t squad_id, Components::FormationType new_type);

    void renderUI();

private:
    BattleMap         battle_map;
    Pathfinding       pathfinding;
    Registry          registry;

    sf::Texture       texture;

    std::vector<uint32_t> selected_squads;
    bool show_imgui = true;
};