#include "SceneHierarhyDisplay.hpp"
#include <imgui.h>


void SceneHierrarhyDisplay::draw()
{
    ImGui::Begin("Scene Hierarchy");
    if (ImGui::TreeNode("Game Objects"))
    {
        if (ImGui::TreeNode("Player"))
        {
            ImGui::Text("Transform");
            ImGui::Text("Sprite Renderer");
            ImGui::Text("Rigidbody");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Enemies"))
        {
            ImGui::Text("Enemy1");
            ImGui::Text("Enemy2");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Environment"))
        {
            ImGui::Text("Ground");
            ImGui::Text("Walls");
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void SceneHierrarhyDisplay::update(const float& delta_time)
{
}
