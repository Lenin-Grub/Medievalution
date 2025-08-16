#pragma once
#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Camera/Camera.h>
#include <ECS/Entity/Entity.hpp>
#include <Gizmos/Gizmos.h>

class SceneDisplay
    : public UI::IDisplay
{
public:
    SceneDisplay(BattleMap& battle_map, Registry& registry, Gizmo& gizmo);
    virtual ~SceneDisplay() = default;

    void draw() override;
    void update(const float& delta_time) override;

    void setSelectedEntity(entt::entity entity);

private:
    BattleMap&  battle_map;
    Registry&   registry;
    Gizmo&      gizmo;

    entt::entity selected_entity = entt::null;
    bool is_brash = false;
};
