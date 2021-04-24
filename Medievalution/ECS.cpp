#include "stdafx.h"
#include "ECS.h"

std::shared_ptr<Entity> ECS::addEntity()
{
    auto entity = std::make_shared<Entity>();
    entity->setEntityManager(this);
    m_entites.push_back(entity);
    return std::move(entity);
}

void ECS::initEntities()
{
    for (const auto& entity : m_entites)
    {
        entity->initEntity();
    }
}

void ECS::initSystems()
{
    for (const auto& system : m_systems)
    {
        system->initSystem();
    }
}

void ECS::updateSystems()
{
    for (const auto& system : m_systems)
    {
        system->updateSystem();
    }
}
