#pragma once
#include "Entity.hpp"

#pragma region Registry


	template<typename TContext>
	inline TContext Registry::addContext(TContext context)
	{
		return regisry->ctx().emplace(context);
	}


	template<typename TContext>
	inline TContext& Registry::getContext()
	{
		return regisry->ctx().get<TContext>();
	}

#pragma endregion


	template<typename TComponent, typename ...Args>
	inline TComponent& Entity::addComponent(Args && ...args)
	{
		auto& reg = registry.getRegistry();
		return reg.emplace<TComponent>(entity, std::forward<Args>(args) ...);
	}


	template<typename TComponent, typename ...Args>
	inline TComponent& Entity::replaceComponent(Args && ...args)
	{
		auto& reg = registry.getRegistry();
		if (reg.all_of<TComponent>(entity))
			return reg.replace<TComponent>(entity, std::forward<Args>(args) ...);
		else
			return reg.emplace<TComponent>(entity, std::forward<Args>(args) ...);
	}


	template<typename TComponent, typename ...Args>
	inline TComponent& Entity::getComponent()
	{
		auto& reg = registry.getRegistry();
		return reg.get<TComponent>(entity);
	}


	template<typename TComponent>
	inline bool Entity::hasComponent()
	{
		auto& reg = registry.getRegistry();
		return reg.all_of<TComponent>(entity);
	}


	template<typename TComponent>
	inline void Entity::removeComponent()
	{
		auto& reg = registry.getRegistry();
		return reg.remove<TComponent>(entity);
	}
#pragma endregion


#pragma region Hide
//template <typename Component, typename... Args>
//void EntityManager::addComponent(entt::entity entity, Args&&... args)
//{
//    if (!hasComponent<Component>(entity))
//        registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
//    else
//    {
//        LOG_ERROR("Component already has");
//        return;
//    }
//}
//
//template <typename Component>
//void EntityManager::deleteComponent(entt::entity entity)
//{
//    if (hasComponent<Component>(entity))
//        registry.remove<Component>(entity);
//}
//
//template <typename Component>
//Component& EntityManager::getComponent(entt::entity entity)
//{
//    return registry.get<Component>(entity);
//}
//
//template <typename Component>
//bool EntityManager::hasComponent(entt::entity entity) const
//{
//    return registry.all_of<Component>(entity);
//}
//
//template<typename Component>
//void EntityManager::addComponent(ComponentTypeIndex index)
//{
//    for (auto entity : selectedEntities)
//    {
//        if (registry.valid(entity))
//            registry.emplace_or_replace<Component>(entity);
//    }
//}
//
//template<typename Component>
//void EntityManager::removeComponent(ComponentTypeIndex index)
//{
//    for (auto entity : selectedEntities)
//    {
//        if (registry.valid(entity))
//            registry.remove<Component>(entity);
//    }
//}
//
//template<typename Component>
//void drawSingleComponentField(entt::registry& registry, entt::entity entity)
//{
//    if (auto* comp = registry.try_get<Component>(entity))
//    {
//        ImGui::PushID(comp);
//
//        if constexpr (std::is_same_v<Component, Components::Position>)
//        {
//            ImGui::Bullet(); ImGui::DragFloat2("Position", &comp->position.x);
//        }
//        else if constexpr (std::is_same_v<Component, Components::Velocity>)
//        {
//            ImGui::Bullet(); ImGui::DragFloat("Speed", &comp->speed);
//        }
//        else if constexpr (std::is_same_v<Component, Components::Sprite>)
//        {
//            ImGui::Bullet(); ImGui::Text("Texture ID");
//        }
//        else if constexpr (std::is_same_v<Component, Components::Selectable>)
//        {
//            ImGui::Bullet(); ImGui::Checkbox("Is Selected", &comp->is_selected);
//        }
//
//        ImGui::PopID();
//    }
//}
//
//template<typename... Components>
//void EntityManager::drawComponentFields(entt::entity entity)
//{
//    (drawSingleComponentField<Components>(registry, entity), ...);
//}
#pragma endregion