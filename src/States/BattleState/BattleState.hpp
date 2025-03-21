#pragma once
#include "../State/State.hpp"

#include "World/Editor/Editor.hpp"
#include "Animation/Animation.hpp"
#include "Pathfinding/Pathfinding.h"
#include "ECS/Entity/Entity.hpp"
#include "ECS/Systems/PathfindingSystem/PathfindingSystem.hpp"
#include "../../Common/Gizmos/Gizmos.h"

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

    // Animator gui methods
    void renderAnimator();

    void renderSpriteSelector();
    void renderAnimationControls();
    void renderAnimationCombo();
    void renderFrameControls();
    void renderPlaybackButtons(int& current_frame, bool& is_payed);
    void renderAddRemoveFrameButtons();
    void renderFrameSelector();
    void renderSpriteSheet();

private:
    Gizmos        gizmos;
    Editor        editor;
    Animator      animator;
    Pathfinding   pathfinding;
    PathfindingSystem pathfindingSystem;
    sf::Sprite    sprite;
    sf::Texture   texture;
    entt::entity  entity;
    int           m_selected_tile_id;
    int           m_animator_tile_selected_id;
    EntityManager entity_manager;

    char animation_name[64] = "";
    std::vector<const char*> items { "Spearman", "Archer" };
    int current_item { 0 };

    bool is_brash;
};