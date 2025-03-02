#pragma once
#include "../State/State.hpp"
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
    void endView(sf::RenderTarget* target);
    void beginView(sf::RenderTarget*& target);

    // Use for ImGui
private:
    void renderEditor();
    void renderMetrics();
    void renderAnimator();

private:
    Editor        editor;
    Animator      animator;
    sf::Sprite    sprite;
    sf::Texture   texture;
    int           m_selected_tile_id;
    int           m_animator_tile_selected_id;
    EntityManager entity_manager;

    char animation_name[64] = "";
    char animation_path[64] = "Spearman.json";
};