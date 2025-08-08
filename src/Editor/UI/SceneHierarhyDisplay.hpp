#pragma once
#include <UI/IDisplay.hpp>
#include <ECS/Entity/Entity.hpp>

class SceneHierrarhyDisplay
	: public UI::IDisplay
{
public:
	SceneHierrarhyDisplay(Registry& registry);
	virtual ~SceneHierrarhyDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

	static uint32_t getSelectedEntityId();

	void setSelectedEntityId(uint32_t id);

	entt::entity getSelectedEntity(Registry& registry);

private:
	Registry& registry;
	static uint32_t selected_entity_Id;
};