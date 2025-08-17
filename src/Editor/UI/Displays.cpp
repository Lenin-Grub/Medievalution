#include "Displays.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <Settings/WindowSettings.h>

Displays::Displays(sf::RenderWindow& window, BattleMap& battle_map, Registry& registry, Gizmo& gizmo)
    : window(window)
    , display_animation(window)
    , display_menu(window, battle_map)
    , display_map_editor(window, battle_map)
    , display_scene(battle_map, registry, gizmo)
    , display_scene_hierarchy(registry)
    , display_inspector(registry)
    , battle_map(battle_map)
    , gizmo (gizmo)
{
    display_scene_hierarchy.setSceneDisplay(&display_scene);
}

bool Displays::initDockSpace()
{
    static bool dockspace_open = true;
    static bool opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove;

        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", &dockspace_open, window_flags);

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        ImGui::PopStyleVar();

    return true;
}

bool Displays::create()
{
    display_menu.draw();
    display_scene_hierarchy.draw();
    display_scene.draw();
    display_log.draw();
    display_inspector.draw();
    display_object_details.draw();
    display_assets.draw();

    if (display_menu.show_demo_window)
        ImGui::ShowDemoWindow(&display_menu.show_demo_window);

    if (display_menu.show_animation_window)
        display_animation.draw();

    if (display_menu.show_map_editor_window)
        display_map_editor.draw();

    if (display_menu.show_metrics)
        metrics();

    return true;
}

void Displays::draw()
{
    initDockSpace();
    initDockingLayout();
    create();

    ImGui::End();
}

    void Displays::initDockingLayout()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MainDockspade");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

            static auto first_time = true;

            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

                auto center       = dockspace_id;
                auto left         = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.20f,  nullptr, &center);
                auto right        = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.50f, nullptr, &center);
                auto bottom       = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.25f,  nullptr, &center);
                auto right_bottom = ImGui::DockBuilderSplitNode(right , ImGuiDir_Down, 0.40f,  nullptr, &right);

                ImGui::DockBuilderDockWindow("Scene Hierarchy", left);
                ImGui::DockBuilderDockWindow("Object Details", right_bottom);
                ImGui::DockBuilderDockWindow(SET_ICON_TEXT((Icon::INSTAGRAM), "Animation editor"), right);
                ImGui::DockBuilderDockWindow(SET_ICON_TEXT((Icon::MAP), " Map editor"), right);
                ImGui::DockBuilderDockWindow("Scene", center);
                ImGui::DockBuilderDockWindow(SET_ICON_TEXT((Icon::SCRIPT), " Logs"), bottom);
                ImGui::DockBuilderDockWindow("Assets", bottom);
                ImGui::DockBuilderDockWindow("Inspector", right);
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }
    }

    void Displays::update(const float& delta_time)
    {
        display_animation.update(delta_time);
        display_map_editor.update(delta_time);
        display_scene.update(delta_time);
        world_mouse_pos = display_scene.world_mouse_pos;
    }

    void Displays::metrics()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Metrics", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

        ImGui::Text("Bla bla bla");

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms. | FPS: %.1f", 1000.0f / io.Framerate ,WindowSettings::getInstance().getFPS());

        ImGui::Columns(4, "table_columns");
        ImGui::Separator();

        ImGui::Text("Coordinates"); ImGui::SameLine();
        ImGui::NextColumn();

        ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
        ImGui::Button("X"); ImGui::SameLine();
        ImGui::PopStyleColor();
        ImGui::Text("%.1f", common::mouse_pos_view.x);
        ImGui::NextColumn();

        ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
        ImGui::Button("Y"); ImGui::SameLine();
        ImGui::PopStyleColor();
        ImGui::Text("%.1f", common::mouse_pos_view.y);
        ImGui::NextColumn();

        ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(23, 68, 210));
        ImGui::Button("Z"); ImGui::SameLine();
        ImGui::PopStyleColor();
        ImGui::Text("%.1f", 1);

        ImGui::Columns(1);

        ImGui::End();
    }
