#include "SceneDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "MapEditorDisplay.hpp"
#include "SceneHierarhyDisplay.hpp"
#include "ToolsState.hpp"

SceneDisplay::SceneDisplay(BattleMap& battle_map, Registry& registry, Gizmo& gizmo)
    : battle_map(battle_map)
    , registry(registry)
    , gizmo(gizmo)
    , is_brash(false)
{
}

void SceneDisplay::draw()
{
    ImGui::Begin("Scene");
    ImGui::Text("Game View - %dx%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

    ImGui::SeparatorText(SET_ICON_TEXT((Icon::TOOL), "Tools"));
    ImGui::Dummy(ImVec2(50.0f, 0.0f));
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::SELECT)))
    {
        ToolsState::instance().setActiveTool(ToolType::Select);
        gizmo.setMode(GizmoMode::None);
        gizmo.deactivate();
        is_brash = false;
        battle_map.setShowPreview(false);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Select Tool (Q)");
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::OPEN_WITHIN)))
    {
        ToolsState::instance().setActiveTool(ToolType::Translate);
        gizmo.setMode(GizmoMode::Translate);
        if (selected_entity != entt::null)
        {
            gizmo.setTarget(selected_entity, registry.getRegistry());
        }
        is_brash = false;
        battle_map.setShowPreview(false);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Translate Tool (W)");
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::UNWRAP)))
    {
        ToolsState::instance().setActiveTool(ToolType::Scale);
        gizmo.setMode(GizmoMode::Scale);
        if (selected_entity != entt::null)
        {
            gizmo.setTarget(selected_entity, registry.getRegistry());
        }
        is_brash = false;
        battle_map.setShowPreview(false);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Scale Tool (E)");
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::UPDATE)))
    {
        ToolsState::instance().setActiveTool(ToolType::Rotate);
        gizmo.setMode(GizmoMode::Rotate);
        if (selected_entity != entt::null)
        {
            gizmo.setTarget(selected_entity, registry.getRegistry());
        }
        is_brash = false;
        battle_map.setShowPreview(false);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Rotate Tool (R)");
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::BRUSH)))
    {
        if (!ToolsState::instance().isBrushActive())
        {
            ToolsState::instance().setActiveTool(ToolType::Brush);
            is_brash = !is_brash;
            battle_map.setShowPreview(is_brash);
        }
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Brush Tool (B)");

    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImVec2 canvas_pos  = ImGui::GetCursorScreenPos();

    if (canvas_size.x > 0 && canvas_size.y > 0)
    {
        static sf::RenderTexture render_texture;
        static ImVec2 last_size(0, 0);

        if (canvas_size.x != last_size.x || canvas_size.y != last_size.y)
        {
            render_texture.create((unsigned int)canvas_size.x, (unsigned int)canvas_size.y);
            last_size = canvas_size;
        }

        sf::Vector2f original_view_size = common::view.getSize();
        float original_aspect = original_view_size.x / original_view_size.y;

        sf::Vector2f new_view_size;
        if (canvas_size.x / canvas_size.y > original_aspect)
        {
            new_view_size.y = original_view_size.y;
            new_view_size.x = original_view_size.y * (canvas_size.x / canvas_size.y);
        }
        else
        {
            new_view_size.x = original_view_size.x;
            new_view_size.y = original_view_size.x * (canvas_size.y / canvas_size.x);
        }

        sf::Vector2f view_center = common::view.getCenter();
        sf::View scaled_view;
        scaled_view.setSize(new_view_size.x, new_view_size.y);
        scaled_view.setCenter(view_center.x, view_center.y);

        render_texture.setView(scaled_view);
        render_texture.clear(sf::Color(50, 50, 50, 255));

        battle_map.draw(render_texture, sf::RenderStates::Default);
        registry.draw(registry.getRegistry(), render_texture);

        ImGui::SetCursorScreenPos(canvas_pos);
        ImGui::InvisibleButton("canvas", canvas_size);

        sf::Vector2f world_mouse_pos;
        bool is_mouse_over_canvas = ImGui::IsItemHovered();

        if (is_mouse_over_canvas)
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 local_pos;
            local_pos.x = mouse_pos.x - canvas_pos.x;
            local_pos.y = mouse_pos.y - canvas_pos.y;

            sf::Vector2f relative_pos(local_pos.x / canvas_size.x, local_pos.y / canvas_size.y);

            world_mouse_pos.x = scaled_view.getCenter().x - new_view_size.x * 0.5f + relative_pos.x * new_view_size.x;
            world_mouse_pos.y = scaled_view.getCenter().y - new_view_size.y * 0.5f + relative_pos.y * new_view_size.y;

            if (battle_map.isShowPreview())
            {
                battle_map.updatePreview(world_mouse_pos);
            }

            if (ImGui::IsMouseDown(0) && battle_map.isShowPreview())
            {
                battle_map.addTile(battle_map.getTileId(), world_mouse_pos);
            }
            else if (ImGui::IsMouseDown(1) && battle_map.isShowPreview())
            {
                battle_map.removeTile(world_mouse_pos);
            }
        }

        if (gizmo.isActive() && selected_entity != entt::null && !ToolsState::instance().isToolActive(ToolType::Brush))
        {
            bool mouse_pressed = ImGui::IsMouseDown(0);
            gizmo.update(registry.getRegistry(), world_mouse_pos, mouse_pressed);
            gizmo.render(registry.getRegistry(), render_texture);
            render_texture.display();
        }

        render_texture.display();

        const sf::Texture& texture = render_texture.getTexture();
        ImTextureID texture_ID = (ImTextureID)(uintptr_t)texture.getNativeHandle();

        ImGui::SetCursorScreenPos(canvas_pos);
        ImGui::Image(texture_ID, canvas_size, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
}

void SceneDisplay::update(const float& delta_time)
{
}

void SceneDisplay::setSelectedEntity(entt::entity entity)
{
    selected_entity = entity;

    if (gizmo.isActive() && selected_entity != entt::null && !ToolsState::instance().isToolActive(ToolType::Brush))
        gizmo.setTarget(selected_entity, registry.getRegistry());

    else if (selected_entity == entt::null)
        gizmo.deactivate();
}
