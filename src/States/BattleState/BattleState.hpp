#pragma once
#include "../State/State.hpp"

#include "World/Editor/Editor.hpp"
#include "Animation/Animation.hpp"
#include "Pathfinding/Pathfinding.h"
#include "ECS/Entity/Entity.hpp"
#include "ECS/Systems/PathfindingSystem/PathfindingSystem.hpp"
#include "../../Common/Gizmos/Gizmos.h"

enum class ToolState
{
    None = 0,
    Translate,
    Rotate,
    Scale,
    Brush,
    Fill,
    Flip,
    Undo,
    Redo
};

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

    // Use for ImGui
private:
    // Editor gui methods
    void renderEditor();

    void renderTools();
    void renderLayersSection();
    void renderTilesetSelector();
    void renderLayerControls();
    void renderTilesSection();
    void renderTilesetTable();

    // Metrics gui method
    void renderMetrics();

private:
    ToolState         tools;
    Gizmos            gizmos;
    Editor            editor;
    Animator          animator;
    Pathfinding       pathfinding;
    Registry          registry;
    AnimationEditor   animaton_editor;


    sf::Sprite        sprite;
    sf::Texture       texture;

    int               m_selected_tile_id;
    bool              is_brash;
};