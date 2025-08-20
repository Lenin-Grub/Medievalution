#include "MenuDisplay.hpp"
#include <imgui.h>



MenuDisplay::MenuDisplay(sf::RenderWindow& window, EditorData& data)
    : window(window)
    , battle_map(data.battle_map)
{
}

void MenuDisplay::draw()
{
    static char filePath[256] = "map_save.json";

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}

            if (ImGui::MenuItem("Open", "Ctrl+O")) 
            {
                battle_map.loadMap(filePath);
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) 
            {
                battle_map.saveMap(filePath);
            }

            if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Exit", "Alt+F4")) { window.close(); }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::SeparatorText("Editors");

            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::INSTAGRAM)," Animation editor"), NULL, &show_animation_window))
                show_animation_window = true;

            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::TOOL)," Map editor"), NULL, &show_map_editor_window)) 
                show_map_editor_window = true;

            ImGui::SeparatorText("Other");

            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::METRIC_UP), "Metrics"), NULL, &show_metrics))
                show_metrics = true;

            ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
            ImGui::MenuItem("Another Window", NULL, &show_another_window);

            ImGui::EndMenu();
        }

        //ImGui::BeginMenu("Settings");
        //ImGui::EndMenu();

        //ImGui::BeginMenu("Help");
        //ImGui::EndMenu();

        ImGui::EndMenuBar();
    }
}

void MenuDisplay::update(const float& delta_time)
{
}
