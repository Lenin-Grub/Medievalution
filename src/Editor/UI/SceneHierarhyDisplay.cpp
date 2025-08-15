#include "SceneHierarhyDisplay.hpp"
#include <imgui.h>
#include <ECS/Entity/Entity.hpp>

uint32_t SceneHierarchyDisplay::selected_entity_Id = SceneHierarchyDisplay::NO_ENTITY_SELECTED;

SceneHierarchyDisplay::SceneHierarchyDisplay(Registry& registry)
    : registry(registry)
{
}

void SceneHierarchyDisplay::draw()
{
    ImGui::Begin("Scene Hierarchy");

    updateDisplayOrder();
    drawEntityList();
    drawContextMenu();
    drawAddEntityPopup();

    ImGui::End();
}

void SceneHierarchyDisplay::updateDisplayOrder()
{
    auto view = registry.getRegistry().view<Components::Identification>();
    std::set<entt::entity> currentEntities;

    for (auto entity : view)
    {
        currentEntities.insert(entity);
    }

    display_order.erase(
        std::remove_if(display_order.begin(), display_order.end(),
            [&currentEntities](entt::entity e) {
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
}

void SceneHierarchyDisplay::drawEntityList()
{
    auto& reg = registry.getRegistry();

    for (size_t i = 0; i < display_order.size(); ++i)
    {
        auto entity = display_order[i];

        if (!reg.valid(entity))
            continue;

        auto& id = reg.get<Components::Identification>(entity);
        std::string displayName = id.name + "##" + std::to_string(static_cast<uint32_t>(entity));

        uint32_t currentEntityId = static_cast<uint32_t>(entity);
        bool isSelected = (selected_entity_Id == currentEntityId);

        ImGui::PushID(currentEntityId);

        if (ImGui::Selectable(displayName.c_str(), isSelected))
        {
            handleEntitySelection(entity);
        }

        ImGui::PopID();

        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
        {
            renaming_entity = entity;
            strncpy(rename_buffer, id.name.c_str(), sizeof(rename_buffer) - 1);
            rename_buffer[sizeof(rename_buffer) - 1] = 0;
        }

        handleEntityRename(entity);

        std::string contextMenuId = "EntityContextMenu##" + std::to_string(currentEntityId);
        if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
        {
            if (ImGui::MenuItem("Delete"))
            {
                handleEntityDeletion(entity);
            }
            ImGui::EndPopup();
        }
    }
}

void SceneHierarchyDisplay::handleEntitySelection(entt::entity entity)
{
    uint32_t entityId = static_cast<uint32_t>(entity);
    selected_entity_Id = entityId;

    if (scene_display) {
        scene_display->setSelectedEntity(entity);
    }

    LOG_INFO("Selected entity ID: {}", entityId);
}

void SceneHierarchyDisplay::handleEntityRename(entt::entity entity)
{
    if (renaming_entity != entity)
        return;

    auto& reg = registry.getRegistry();
    auto& id = reg.get<Components::Identification>(entity);

    ImGui::SetKeyboardFocusHere();
    std::string inputName = "##Rename" + std::to_string(static_cast<uint32_t>(entity));

    if (ImGui::InputText(inputName.c_str(), rename_buffer, sizeof(rename_buffer),
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

void SceneHierarchyDisplay::handleEntityDeletion(entt::entity entity)
{
    uint32_t entityId = static_cast<uint32_t>(entity);

    registry.getRegistry().destroy(entity);

    if (selected_entity_Id == entityId)
        selected_entity_Id = NO_ENTITY_SELECTED;

    if (renaming_entity == entity)
        renaming_entity = entt::null;
}

void SceneHierarchyDisplay::drawContextMenu()
{
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered())
        ImGui::OpenPopup("AddGameObjectMenu");
}

void SceneHierarchyDisplay::drawAddEntityPopup()
{
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
}

void SceneHierarchyDisplay::update(const float& delta_time)
{

}

uint32_t SceneHierarchyDisplay::getSelectedEntityId()
{
    return selected_entity_Id;
}

void SceneHierarchyDisplay::setSelectedEntityId(uint32_t id)
{
    selected_entity_Id = id;
}

entt::entity SceneHierarchyDisplay::getSelectedEntity(Registry& registry)
{
    if (selected_entity_Id == NO_ENTITY_SELECTED)
        return entt::null;

    entt::entity entity = entt::entity(selected_entity_Id);
    return registry.getRegistry().valid(entity) ? entity : entt::null;
}

void SceneHierarchyDisplay::setSceneDisplay(SceneDisplay *scene_display)
{ 
    this->scene_display = scene_display;
}
