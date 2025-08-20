#pragma once
#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Camera/Camera.h>
#include <ECS/Entity/Entity.hpp>
#include <Gizmos/Gizmos.h>
#include <imgui.h>
#include "ToolsState.hpp"
#include "EditorData.hpp"


class SceneDisplay
    : public UI::IDisplay
{
public:
    SceneDisplay(EditorData& data);
    virtual ~SceneDisplay() = default;

    void draw() override;
    void drawDisplay();
    void update(const float& delta_time) override;
    
    bool isHover() const;

    sf::Vector2f getWorldMousePos() const;
    void setSelectedEntity(entt::entity entity);

private:  
    void drawToolbar();
    void drawTexture(sf::RenderTexture& render_texture, const sf::View& view);
    void drawIsometricGrid(sf::RenderTarget& target, const sf::View& view);
    
    void setActiveTool(ToolType tool, GizmoMode gizmo_mode, bool should_set_target);
    void setupRenderTexture(sf::RenderTexture& render_texture, const ImVec2& canvas_size, ImVec2& last_size);
    
    sf::View calculateView(const ImVec2& canvas_size) const;
    sf::Vector2f calculateWorldMousePos(const ImVec2& mouse_pos,const ImVec2& canvas_pos,const ImVec2& canvas_size,const sf::View& scaled_view);
    
    void handlenput(const ImVec2& canvas_pos, const ImVec2& canvas_size, const sf::View& view);
    void presentTextureToImGui(const sf::Texture& texture, const ImVec2& canvas_pos, const ImVec2& canvas_size);

private:
    sf::Vector2f world_mouse_pos;
    BattleMap&   battle_map;
    Registry&    registry;
    Gizmo&       gizmo;

    entt::entity selected_entity = entt::null;
    bool show_grid;
    bool is_hovered;

    sf::View scaled_view;
};
