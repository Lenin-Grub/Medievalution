#pragma once
#include "State.hpp"
#include "Pathfinding/Pathfinding.h"
#include "World/Editor/Board.hpp"
#include "World/Editor/Editor.hpp"
#include "World/Editor/SpriteSheet.hpp"
#include "Animation/Animation.hpp"

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
    SpriteSheet m_sprite_sheet;
    Board m_board;
    std::unique_ptr<Animator> animator;
    int m_selected_tile_id;
    sf::Sprite sprite;
    sf::Texture texture;
    Editor editor;
};