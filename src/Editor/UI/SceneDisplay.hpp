#pragma once
#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Camera/Camera.h>
#include <ECS/Entity/Entity.hpp>

class SceneDisplay
	: public UI::IDisplay
{
public:
	SceneDisplay(BattleMap& battle_map, Registry& registry);
	virtual ~SceneDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:
	BattleMap& battle_map;
	Registry& registry;

	bool              is_brash;
	bool              show_editor_window = false;
	bool              show_metrics_window = false;
	bool              show_animator_window = false;
};