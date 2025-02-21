#pragma once
#include "State.hpp"
#include "Pathfinding/Pathfinding.h"
#include "World/Editor/Editor.hpp"
#include "Animation/Animation.hpp"
#include "GUI/GUIBattleState.h" 

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

public:
    Pathfinding pathfinding;
    Editor      editor;
    Animator    animator;
    sf::Sprite  sprite;
    sf::Texture texture;
    int         m_selected_tile_id;
    int         m_animator_tile_selected_id;
};