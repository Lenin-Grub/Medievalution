#include "ObjectDetailsDisplay.hpp"

void ObjectDetailsDisplay::draw()
{
    static float windowWidth;
    static float windowHeight;

    ImGui::Begin("Object Details");
    ImGui::SeparatorText("Default Window Parameters");

    ImGui::Dummy(ImVec2(0.0f, 2 * ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight()));
    ImGui::Text("Window Size");
    ImGui::SameLine();

    ImGui::PushItemWidth(128.f);

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
    ImGui::Button("X"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::SameLine();


    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    ImGui::Button("Y"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2 * ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight()));
    ImGui::Text("Window Flags");
    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImGui::Button("X");
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    ImGui::InputFloat("##pos_x", &windowWidth, 1.0f, 1.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    ImGui::Button("Y");
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    ImGui::InputFloat("##pos_y", &windowHeight, 1.0f, 1.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("Position");

    ImGui::End();

}

void ObjectDetailsDisplay::update(const float& delta_time)
{
}
