#pragma once
#include <UI/IDisplay.hpp>
#include <ECS/Entity/Entity.hpp>
#include "SceneDisplay.hpp"

class SceneHierarchyDisplay 
    : public UI::IDisplay
{
public:
    SceneHierarchyDisplay(EditorData& data);
    ~SceneHierarchyDisplay() override = default;

    void draw() override;
    void update(const float& delta_time) override;

    static uint32_t getSelectedEntityId();
    void setSelectedEntityId(uint32_t id);
    entt::entity getSelectedEntity(Registry& registry);

    void setSceneDisplay(SceneDisplay* scene_display);

private:
    void drawEntityList();
    void drawContextMenu();
    void drawAddEntityPopup();
    void updateDisplayOrder();
    void handleEntitySelection(entt::entity entity);
    void handleEntityRename(entt::entity entity);
    void handleEntityDeletion(entt::entity entity);

private:
    static uint32_t selected_entity_Id;
    static constexpr uint32_t NO_ENTITY_SELECTED = UINT32_MAX;
    
    std::vector<entt::entity> display_order;
    char rename_buffer[128] = { 0 };
    
    entt::entity renaming_entity = entt::null;
    SceneDisplay* scene_display = nullptr;
    Registry& registry;
};