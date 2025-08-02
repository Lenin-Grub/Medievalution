#include "InspectorDisplay.hpp"
#include <imgui.h>

void InspectorDisplay::draw()
{
    ImGui::Begin("Inspector");
    ImGui::Text("Selected: Player");
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputFloat3("Position", &float_value);
        ImGui::InputFloat3("Rotation", &float_value);
        ImGui::InputFloat3("Scale", &float_value);
    }
    if (ImGui::CollapsingHeader("Components"))
    {
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("component_popup");
        }

        if (ImGui::BeginPopup("component_popup"))
        {
            if (ImGui::MenuItem("Sprite Renderer")) {}
            if (ImGui::MenuItem("Rigidbody")) {}
            if (ImGui::MenuItem("Collider")) {}
            if (ImGui::MenuItem("Script")) {}
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void InspectorDisplay::update(const float& delta_time)
{
}
