#include "ObjectDetailsDisplay.hpp"
#include <imgui.h>

void ObjectDetailsDisplay::draw()
{
    ImGui::Begin("Object Details");
    ImGui::Text("Transform Component");
    ImGui::DragFloat3("Position", &float_value, 0.1f);
    ImGui::DragFloat3("Rotation", &float_value, 0.1f);
    ImGui::DragFloat3("Scale", &float_value, 0.1f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Sprite Renderer");
    ImGui::Checkbox("Enabled", &bool_value);
    ImGui::Combo("Sprite", &current_item, ITEMS, IM_ARRAYSIZE(ITEMS));
    ImGui::SliderFloat("Opacity", &float_value, 0.0f, 1.0f);
    ImGui::End();
}

void ObjectDetailsDisplay::update(const float& delta_time)
{
}
