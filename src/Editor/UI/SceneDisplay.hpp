#pragma once
#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Camera/Camera.h>

class SceneDisplay
	: public UI::IDisplay
{
public:
	SceneDisplay(BattleMap& battle_map);
	virtual ~SceneDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:
	BattleMap& battle_map;
};