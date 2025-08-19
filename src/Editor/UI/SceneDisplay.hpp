#pragma once
#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Camera/Camera.h>
#include <ECS/Entity/Entity.hpp>
#include <Gizmos/Gizmos.h>
#include <imgui.h>

class SceneDisplay
    : public UI::IDisplay
{
public:
    SceneDisplay(BattleMap& battle_map, Registry& registry, Gizmo& gizmo);
    virtual ~SceneDisplay() = default;

    void draw() override;
    void update(const float& delta_time) override;

    void setSelectedEntity(entt::entity entity);
    sf::Vector2f calculateWorldMousePos(const ImVec2& mouse_pos,const ImVec2& canvas_pos,const ImVec2& canvas_size,const sf::View& scaled_view);

    void drawIsometricGrid(sf::RenderTarget& target, const sf::View& view);

    sf::Vector2f world_mouse_pos;

private:
    BattleMap&  battle_map;
    Registry&   registry;
    Gizmo&      gizmo;

    entt::entity selected_entity = entt::null;
    bool is_brash = false;
    bool show_grid;
};
