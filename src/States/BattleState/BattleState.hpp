#pragma once
#include "../State/State.hpp"
#include "Pathfinding/Pathfinding.h"
#include "World/Editor/Editor.hpp"
#include "Animation/Animation.hpp"
#include "ECS/Entity/Entity.hpp"

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
    void renderEditor();
    void renderMetrics();
    void renderAnimator();

private:
    Pathfinding   pathfinding;
    Editor        editor;
    Animator      animator;
    sf::Sprite    sprite;
    sf::Texture   texture;
    int           m_selected_tile_id;
    int           m_animator_tile_selected_id;
    EntityManager entity_manager;
};