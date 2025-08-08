#include "SceneHierarhyDisplay.hpp"
#include <imgui.h>
#include <ECS/Entity/Entity.hpp>

uint32_t SceneHierrarhyDisplay::selected_entity_Id = UINT32_MAX;

SceneHierrarhyDisplay::SceneHierrarhyDisplay(Registry& registry)
    : registry(registry)
{
}

void SceneHierrarhyDisplay::draw()
{
    ImGui::Begin("Scene Hierarchy");

    auto view = registry.getRegistry().view<Components::Identification>();

    static entt::entity renaming_entity = entt::null;
    static char rename_buffer[128] = { 0 };
    static std::vector<entt::entity> display_order;

    if (display_order.empty())
    {
        for (auto entity : view)
        {
            display_order.push_back(entity);
        }
    }

    std::set<entt::entity> currentEntities;
    for (auto entity : view)
    {
        currentEntities.insert(entity);
    }

    display_order.erase(
        std::remove_if(display_order.begin(), display_order.end(),
            [&currentEntities](entt::entity e) 
            {
                return !currentEntities.count(e);
            }),
        display_order.end()
    );

    for (auto entity : view)
    {
        if (std::find(display_order.begin(), display_order.end(), entity) == display_order.end())
        {
            display_order.push_back(entity);
        }
    }

    for (size_t i = 0; i < display_order.size(); ++i)
    {
        auto entity = display_order[i];

        if (!registry.getRegistry().valid(entity))
            continue;

        auto& id = registry.getRegistry().get<Components::Identification>(entity);
        std::string displayName = id.name + "##" + std::to_string(static_cast<uint32_t>(entity));

        uint32_t currentEntityId = static_cast<uint32_t>(entity);
        bool isSelected = (selected_entity_Id == currentEntityId);

        ImGui::PushID(currentEntityId);
        if (ImGui::Selectable(displayName.c_str(), isSelected))
        {
            selected_entity_Id = currentEntityId;
            LOG_INFO("Selected entity ID: {0})", currentEntityId);
        }
        ImGui::PopID();

        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
        {
            renaming_entity = entity;
            strncpy(rename_buffer, id.name.c_str(), sizeof(rename_buffer) - 1);
            rename_buffer[sizeof(rename_buffer) - 1] = 0;
        }

        if (renaming_entity == entity)
        {
            ImGui::SetKeyboardFocusHere();
            std::string inputName = "##Rename" + std::to_string(currentEntityId);
            if (ImGui::InputText(inputName.c_str(),
                rename_buffer, sizeof(rename_buffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                id.name = std::string(rename_buffer);
                renaming_entity = entt::null;
            }

            if (!ImGui::IsItemActive() && !ImGui::IsMouseClicked(0))
            {
                renaming_entity = entt::null;
            }
        }

        if (ImGui::BeginPopupContextItem(("EntityContextMenu##" + std::to_string(currentEntityId)).c_str()))
        {
            if (ImGui::MenuItem("Delete"))
            {
                registry.getRegistry().destroy(entity);
                if (selected_entity_Id == currentEntityId)
                    selected_entity_Id = UINT32_MAX;
                if (renaming_entity == entity)
                    renaming_entity = entt::null;
            }
            ImGui::EndPopup();
        }
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered())
        ImGui::OpenPopup("AddGameObjectMenu");

    if (ImGui::BeginPopup("AddGameObjectMenu"))
    {
        if (ImGui::MenuItem("Add Game Object"))
        {
            entt::entity new_entity = registry.createEntity();
            registry.getRegistry().emplace<Components::Identification>(new_entity, "GameObject");
            registry.getRegistry().emplace<Components::Position>(new_entity);
            LOG_INFO("Created new entity ID: {}", static_cast<uint32_t>(new_entity));
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void SceneHierrarhyDisplay::update(const float& delta_time)
{
}

uint32_t SceneHierrarhyDisplay::getSelectedEntityId()
{
    return selected_entity_Id;
}

void SceneHierrarhyDisplay::setSelectedEntityId(uint32_t id)
{
    selected_entity_Id = id;
}

entt::entity SceneHierrarhyDisplay::getSelectedEntity(Registry& registry)
{
    if (selected_entity_Id == UINT32_MAX)
        return entt::null;

    // Преобразуем ID обратно в entity
    entt::entity entity = entt::entity(selected_entity_Id);
    if (registry.getRegistry().valid(entity))
        return entity;

    return entt::null;
}
