#pragma once
#include "State.hpp"
#include "../Camera/Camera.h"
#include "../Pathfinding/Pathfinding.h"
#include "../World/Editor/Board.hpp"
#include "../World/Editor/SpriteSheet.hpp"

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
    Pathfinding pathfinding;
    Camera camera;

    SpriteSheet m_sprite_sheet;
    Board m_board{ 32, 50, 50 };
    int m_selected_tile_id = 0;
};