#include "SceneDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "MapEditorDisplay.hpp"
#include "SceneHierarhyDisplay.hpp"

SceneDisplay::SceneDisplay(BattleMap& battle_map, Registry& registry, Gizmo& gizmo)
    : battle_map(battle_map)
    , registry(registry)
    , gizmo(gizmo)
    , show_grid(true)
    , is_hovered(false)
{
}

void SceneDisplay::update(const float& delta_time)
{
    if (gizmo.isActive() && selected_entity != entt::null && !ToolsState::instance().isToolActive(ToolType::Brush))
        gizmo.update(registry.getRegistry(), world_mouse_pos, ImGui::IsMouseDown(0));
}

void SceneDisplay::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::GLOBE), "Scene"));
    ImGui::Text("Game View - %dx%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

    drawToolbar();
    drawDisplay();

    this->is_hovered = ImGui::IsWindowHovered();

    ImGui::End();
}

void SceneDisplay::drawDisplay()
{
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImVec2 canvas_pos  = ImGui::GetCursorScreenPos();

    if (canvas_size.x > 0 && canvas_size.y > 0)
    {
        static sf::RenderTexture render_texture;
        static ImVec2 last_size(0, 0);

        setupRenderTexture(render_texture, canvas_size, last_size);
        scaled_view = calculateView(canvas_size);

        drawTexture(render_texture, scaled_view);

        handlenput(canvas_pos, canvas_size, scaled_view);

        if (gizmo.isActive() && selected_entity != entt::null && !ToolsState::instance().isToolActive(ToolType::Brush))
            gizmo.render(registry.getRegistry(), render_texture);

        render_texture.display();

        presentTextureToImGui(render_texture.getTexture(), canvas_pos, canvas_size);
    }
}

void SceneDisplay::drawToolbar()
{
    ImGui::SeparatorText(SET_ICON_TEXT((Icon::TOOL), "Tools"));
    ImGui::Dummy(ImVec2(50.0f, 0.0f));
    ImGui::SameLine();

    // Select Tool
    if (ImGui::Button(SET_ICON(Icon::SELECT)))
    {
        setActiveTool(ToolType::Select, GizmoMode::None, false);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select Tool (Q)");

    ImGui::SameLine();

    // Translate Tool
    if (ImGui::Button(SET_ICON(Icon::OPEN_WITHIN)))
    {
        setActiveTool(ToolType::Translate, GizmoMode::Translate, true);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Translate Tool (W)");

    ImGui::SameLine();

    // Scale Tool
    if (ImGui::Button(SET_ICON(Icon::UNWRAP)))
    {
        setActiveTool(ToolType::Scale, GizmoMode::Scale, true);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale Tool (E)");

    ImGui::SameLine();

    // Rotate Tool
    if (ImGui::Button(SET_ICON(Icon::UPDATE)))
    {
        setActiveTool(ToolType::Rotate, GizmoMode::Rotate, true);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Rotate Tool (R)");

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(50, 0));
    ImGui::SameLine();

    // Brush Tool
    if (ImGui::Button(SET_ICON(Icon::BRUSH)))
    {
        ToolsState::instance().setActiveTool(ToolType::Brush);
        //is_brash = !is_brash;
        battle_map.setShowPreview(true);
    }

    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brush Tool (B)");

    ImGui::SameLine();

    // Fill Tool
    if (ImGui::Button(SET_ICON(Icon::FILL)))
    {
        battle_map.fill(1);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Fill Tool (G)");
}

void SceneDisplay::setActiveTool(ToolType tool, GizmoMode gizmo_mode, bool should_set_target)
{
    ToolsState::instance().setActiveTool(tool);
    gizmo.setMode(gizmo_mode);
    if (should_set_target && selected_entity != entt::null)
    {
        gizmo.setTarget(selected_entity, registry.getRegistry());
    }
    else
    {
        gizmo.deactivate();
    }
    //is_brash = false;
    battle_map.setShowPreview(false);
}

void SceneDisplay::setupRenderTexture(sf::RenderTexture& render_texture, const ImVec2& canvas_size, ImVec2& last_size)
{
    if (canvas_size.x != last_size.x || canvas_size.y != last_size.y)
    {
        render_texture.create(static_cast<unsigned int>(canvas_size.x), static_cast<unsigned int>(canvas_size.y));
        last_size = canvas_size;
    }
}

sf::View SceneDisplay::calculateView(const ImVec2& canvas_size) const
{
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
    scaled_view.setSize(new_view_size);
    scaled_view.setCenter(view_center);
    return scaled_view;
}

void SceneDisplay::drawTexture(sf::RenderTexture& render_texture, const sf::View& view)
{
    render_texture.setView(view);
    render_texture.clear(sf::Color(50, 50, 50, 255));

    battle_map.draw(render_texture, sf::RenderStates::Default);
    registry.draw(registry.getRegistry(), render_texture);

    if (show_grid)
    {
        drawIsometricGrid(render_texture, view);
    }
}

void SceneDisplay::handlenput(const ImVec2& canvas_pos, const ImVec2& canvas_size, const sf::View& view)
{
    ImGui::SetCursorScreenPos(canvas_pos);
    ImGui::InvisibleButton("Canvas", canvas_size);

    if (ImGui::IsItemHovered())
    {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        world_mouse_pos = calculateWorldMousePos(mouse_pos, canvas_pos, canvas_size, view);

        if (battle_map.isShowPreview())
        {
            battle_map.updatePreview(world_mouse_pos);

            if (ImGui::IsMouseDown(0))
            {
                battle_map.addTile(battle_map.getTileId(), world_mouse_pos);
            }
            else if (ImGui::IsMouseDown(1))
            {
                battle_map.removeTile(world_mouse_pos);
            }
        }
    }
}

void SceneDisplay::presentTextureToImGui(const sf::Texture& texture, const ImVec2& canvas_pos, const ImVec2& canvas_size)
{
    ImTextureID texture_ID = (ImTextureID)(uintptr_t)texture.getNativeHandle();
    ImGui::SetCursorScreenPos(canvas_pos);
    ImGui::Image(texture_ID, canvas_size, ImVec2(0, 1), ImVec2(1, 0));
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
    ImVec2 local_pos = { mouse_pos.x - canvas_pos.x, mouse_pos.y - canvas_pos.y };
    sf::Vector2f relative_pos(local_pos.x / canvas_size.x, local_pos.y / canvas_size.y);

    sf::Vector2f view_center = scaled_view.getCenter();
    sf::Vector2f view_size   = scaled_view.getSize();

    sf::Vector2f world_mouse_pos;
    world_mouse_pos.x = view_center.x - view_size.x * 0.5f + relative_pos.x * view_size.x;
    world_mouse_pos.y = view_center.y - view_size.y * 0.5f + relative_pos.y * view_size.y;

    return world_mouse_pos;
}

void SceneDisplay::drawIsometricGrid(sf::RenderTarget& target, const sf::View& view)
{
    sf::Vector2f view_center = view.getCenter();
    sf::Vector2f view_size   = view.getSize();

    float left   = view_center.x - view_size.x * 0.5f;
    float right  = view_center.x + view_size.x * 0.5f;
    float top    = view_center.y - view_size.y * 0.5f;
    float bottom = view_center.y + view_size.y * 0.5f;

    const float tile_width  = 64.0f;
    const float tile_height = 32.0f;
    const sf::Color grid_color(100, 100, 100, 100);

    std::vector<sf::Vertex> grid_lines;

    int start_x = static_cast<int>(left   / tile_width)  - 2;
    int end_x   = static_cast<int>(right  / tile_width)  + 2;
    int start_y = static_cast<int>(top    / tile_height) - 2;
    int end_y   = static_cast<int>(bottom / tile_height) + 2;

    for (int x = start_x; x <= end_x; ++x)
    {
        for (int y = start_y; y <= end_y; ++y)
        {
            sf::Vector2f top_point(x * tile_width, y * tile_height);
            sf::Vector2f left_point((x - 0.5f) * tile_width, (y + 0.5f) * tile_height);
            sf::Vector2f right_point((x + 0.5f) * tile_width, (y + 0.5f) * tile_height);
            sf::Vector2f bottom_point(x * tile_width, (y + 1) * tile_height);

            // Top -> Left
            grid_lines.push_back(sf::Vertex(top_point, grid_color));
            grid_lines.push_back(sf::Vertex(left_point, grid_color));

            // Left -> Bottom
            grid_lines.push_back(sf::Vertex(left_point, grid_color));
            grid_lines.push_back(sf::Vertex(bottom_point, grid_color));

            // Bottom -> Right
            grid_lines.push_back(sf::Vertex(bottom_point, grid_color));
            grid_lines.push_back(sf::Vertex(right_point, grid_color));

            // Right -> Top
            grid_lines.push_back(sf::Vertex(right_point, grid_color));
            grid_lines.push_back(sf::Vertex(top_point, grid_color));
        }
    }

    if (!grid_lines.empty())
        target.draw(grid_lines.data(), grid_lines.size(), sf::Lines);
}

bool SceneDisplay::isHover() const
{
    return is_hovered;
}

sf::Vector2f SceneDisplay::getWorldMousePos() const
{
    return world_mouse_pos;
}