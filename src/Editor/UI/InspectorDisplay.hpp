#pragma once
#include <UI/IDisplay.hpp>
#include <ECS/Entity/Entity.hpp>
#include "EditorData.hpp"

class Registry;

class InspectorDisplay 
    : public UI::IDisplay
{
public:
    InspectorDisplay(EditorData& data);
    ~InspectorDisplay() override = default;

    void draw() override;
    void drawIdentification(entt::registry& reg, entt::entity selected, uint32_t selectedId);
    void update(const float& delta_time) override;

    // Helper methods
private:
    void drawAddComponentButton();
    void drawComponentListPopup(entt::entity selected);
    void drawTransformSection(entt::entity selected);
    void drawMovementSection(entt::entity selected);
    void drawSpriteSection(entt::entity selected);
    void drawIndividualComponents(entt::entity selected);
    void drawStateSection(entt::entity selected);
    void drawFormation(entt::entity selected);

    // Component-specific drawing methods
private:
    void drawSelectableComponent(entt::entity selected);
    void drawPositionComponent(entt::entity selected);
    void drawScaleComponent(entt::entity selected);
    void drawRotationComponent(entt::entity selected);
    void drawControlComponent(entt::entity selected);
    void drawVelocityComponent(entt::entity selected);
    void drawPathComponent(entt::entity selected);
    void drawAnimationComponent(entt::entity selected);

private:
    Registry& registry;
    static constexpr uint32_t NO_ENTITY_SELECTED = UINT32_MAX;

    bool is_renaming_name  = false;
    bool is_renaming_group = false;

    char rename_buffer_name[256]  = "";
    char rename_buffer_group[256] = "";

    entt::entity renaming_entity = entt::null;
};