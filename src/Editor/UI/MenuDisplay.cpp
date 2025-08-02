#include "MenuDisplay.hpp"
#include <imgui.h>


MenuDisplay::MenuDisplay(sf::RenderWindow& window, BattleMap& battle_map)
    : window(window)
    , battle_map(battle_map)
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
            ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
            ImGui::MenuItem("Another Window", NULL, &show_another_window);
            if (ImGui::MenuItem("Animation Window", NULL, show_animation_window)) { show_animation_window = true; }
            if (ImGui::MenuItem("Map Editor Window", NULL, show_map_editor_window)) { show_map_editor_window = true; }
            if (ImGui::MenuItem("Metrics", NULL, show_metrics)) { show_metrics = true; }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void MenuDisplay::update(const float& delta_time)
{
}
