#include "LogDisplay.hpp"
#include <imgui.h>

void LogDisplay::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::SCRIPT), " Logs"));
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "[INFO] Application started");
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "[WARNING] Low memory");
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "[ERROR] Failed to load asset");
    ImGui::Text("Frame time: %.2f ms", ImGui::GetIO().DeltaTime * 1000.0f);
    ImGui::End();
}

void LogDisplay::update(const float& delta_time)
{
}
