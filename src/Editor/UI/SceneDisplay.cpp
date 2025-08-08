#include "SceneDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "MapEditorDisplay.hpp"

SceneDisplay::SceneDisplay(BattleMap& battle_map, Registry& registry)
    : battle_map(battle_map)
    , registry(registry)
{
}

void SceneDisplay::draw()
{
    ImGui::Begin("Scene");
    ImGui::Text("Game View - %dx%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

        ImGui::SeparatorText(SET_ICON_TEXT((Icon::TOOL), "Tools"));
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        sf::Color green(40, 159, 49, 255);
        ImVec4 imVecColor(
            green.r / 255.0f,
            green.g / 255.0f,
            green.b / 255.0f,
            green.a / 255.0f);

        auto renderToolButton = [&](Icon icon, const char* id, ToolState tool, GizmoMode mode)
            {
                //bool isPressed = tools == tool;
                bool isPressed = true;
                if (isPressed)
                    ImGui::PushStyleColor(ImGuiCol_Button, imVecColor);

                if (ImGui::Button((ICON::getStr(icon) + std::string(" ##") + id).c_str()))
                {
                    if (tool == ToolState::Brush)
                    {
                        is_brash = !is_brash;
                        battle_map.setShowPreview(true);
                    }
                    else
                    {
                        is_brash = false;
                        battle_map.setShowPreview(false);
                    }

                    //tools = tool;
                    //gizmos.mode = mode;
                }

                if (isPressed)
                    ImGui::PopStyleColor();
            };

        // Select
        renderToolButton(Icon::SELECT, "Select", ToolState::None, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Move
        renderToolButton(Icon::OPEN_WITHIN, "Move", ToolState::Translate, GizmoMode::Translate);
        ImGui::SameLine(0.0f, 1.0f);

        // Scale
        renderToolButton(Icon::UNWRAP, "Scale", ToolState::Scale, GizmoMode::Scale);
        ImGui::SameLine(0.0f, 1.0f);

        // Rotate
        renderToolButton(Icon::UPDATE, "Rotate", ToolState::Rotate, GizmoMode::Rotate);
        ImGui::SameLine(0.0f, 1.0f);

        // Flip
        renderToolButton(Icon::FLIP_HORiZONTAL, "Flip", ToolState::Flip, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Space
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        // Brush
        renderToolButton(Icon::BRUSH, "Brush", ToolState::Brush, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Fill
        renderToolButton(Icon::FILL, "Fill", ToolState::Fill, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Space
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        // Undo
        renderToolButton(Icon::UNDO, "Undo", ToolState::Undo, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        //Redo
        renderToolButton(Icon::REDO, "Redo", ToolState::Redo, GizmoMode::None);

        // Gizmo
        //gizmos.drawImGui();

    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    if (canvas_size.x > 0 && canvas_size.y > 0)
    {
        static sf::RenderTexture render_texture;
        if (render_texture.getSize().x != (unsigned int)canvas_size.x || render_texture.getSize().y != (unsigned int)canvas_size.y) 
        {
            render_texture.create((unsigned int)canvas_size.x, (unsigned int)canvas_size.y);
        }

        sf::View scene_view;
        scene_view.reset(sf::FloatRect(0, 0, canvas_size.x, canvas_size.y));

        sf::Vector2f camera_offset = common::view.getCenter() - (sf::Vector2f(canvas_size.x, canvas_size.y) * 0.5f);
        float zoom_factor_x = common::view.getSize().x / canvas_size.x;
        float zoom_factor_y = common::view.getSize().y / canvas_size.y;

        scene_view.move(camera_offset);
        scene_view.zoom(zoom_factor_x);

        render_texture.setView(scene_view);
        render_texture.clear(sf::Color(50, 50, 50, 255));

        battle_map.draw(render_texture, sf::RenderStates::Default);
        registry.draw(registry.getRegistry(), render_texture);

        render_texture.display();
        render_texture.setView(render_texture.getDefaultView());

        const sf::Texture& texture = render_texture.getTexture();
        ImTextureID texture_ID = (ImTextureID)(uintptr_t)texture.getNativeHandle();

        ImGui::Image(texture_ID, ImVec2(canvas_size.x, canvas_size.y),ImVec2(0, 1),ImVec2(1, 0));
    }

    ImGui::End();
}

void SceneDisplay::update(const float& delta_time)
{
}