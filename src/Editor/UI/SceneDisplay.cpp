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
    , show_grid(true)
{
}

void SceneDisplay::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::GLOBE), "Scene"));
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
    ImGui::Dummy(ImVec2(50,0));
    ImGui::SameLine();

    if (ImGui::Button(SET_ICON(Icon::BRUSH)))
    {
        if (!ToolsState::instance().isToolActive(ToolType::Brush))
        {
            ToolsState::instance().setActiveTool(ToolType::Brush);
            is_brash = !is_brash;
            battle_map.setShowPreview(is_brash);
        }
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Brush Tool (B)");

    ImGui::SameLine();
    if (ImGui::Button(SET_ICON(Icon::FILL)))
    {
        battle_map.fill(1);
    }

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Fill Tool (G)");

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

        if (show_grid)
        {
            drawIsometricGrid(render_texture, scaled_view);
        }

        ImGui::SetCursorScreenPos(canvas_pos);
        ImGui::InvisibleButton("canvas", canvas_size);

        bool is_mouse_over_canvas = ImGui::IsItemHovered();

        if (is_mouse_over_canvas)
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            world_mouse_pos = calculateWorldMousePos(mouse_pos, canvas_pos, canvas_size, scaled_view);

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

sf::Vector2f SceneDisplay::calculateWorldMousePos(const ImVec2& mouse_pos, const ImVec2& canvas_pos, const ImVec2& canvas_size, const sf::View& scaled_view)
{
    ImVec2 local_pos;
    local_pos.x = mouse_pos.x - canvas_pos.x;
    local_pos.y = mouse_pos.y - canvas_pos.y;

    sf::Vector2f relative_pos(local_pos.x / canvas_size.x, local_pos.y / canvas_size.y);

    sf::Vector2f world_mouse_pos;
    world_mouse_pos.x = scaled_view.getCenter().x - scaled_view.getSize().x * 0.5f + relative_pos.x * scaled_view.getSize().x;
    world_mouse_pos.y = scaled_view.getCenter().y - scaled_view.getSize().y * 0.5f + relative_pos.y * scaled_view.getSize().y;

    return world_mouse_pos;
}

void SceneDisplay::drawIsometricGrid(sf::RenderTarget& target, const sf::View& view)
{
    sf::Vector2f view_center = view.getCenter();
    sf::Vector2f view_size = view.getSize();

    float left   = view_center.x - view_size.x * 0.5f;
    float right  = view_center.x + view_size.x * 0.5f;
    float top    = view_center.y - view_size.y * 0.5f;
    float bottom = view_center.y + view_size.y * 0.5f;

    const float tile_width = 64.0f;
    const float tile_height = 32.0f;
    const sf::Color grid_color(100, 100, 100, 100);


    std::vector<sf::Vertex> grid_lines;

    int start_x = (int)(left / tile_width)  - 2;
    int end_x   = (int)(right / tile_width) + 2;
    int start_y = (int)(top / tile_height)  - 2;
    int end_y   = (int)(bottom / tile_height) + 2;

    for (int x = start_x; x <= end_x; ++x)
    {
        for (int y = start_y; y <= end_y; ++y)
        {
            sf::Vector2f top_point(x * tile_width, y * tile_height);
            sf::Vector2f left_point((x - 0.5f) * tile_width, (y + 0.5f) * tile_height);
            sf::Vector2f right_point((x + 0.5f) * tile_width, (y + 0.5f) * tile_height);
            sf::Vector2f bottom_point(x * tile_width, (y + 1) * tile_height);

            grid_lines.push_back(sf::Vertex(top_point, grid_color));
            grid_lines.push_back(sf::Vertex(left_point, grid_color));

            grid_lines.push_back(sf::Vertex(left_point, grid_color));
            grid_lines.push_back(sf::Vertex(bottom_point, grid_color));

            grid_lines.push_back(sf::Vertex(bottom_point, grid_color));
            grid_lines.push_back(sf::Vertex(right_point, grid_color));

            grid_lines.push_back(sf::Vertex(right_point, grid_color));
            grid_lines.push_back(sf::Vertex(top_point, grid_color));
        }
    }

    if (!grid_lines.empty())
    {
        target.draw(grid_lines.data(), grid_lines.size(), sf::Lines);
    }
}