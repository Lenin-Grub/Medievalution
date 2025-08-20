#pragma once
#include <World/BattleMap/BattleMap.hpp>
#include <Gizmos/Gizmos.h>
#include <Camera/Camera.h>
#include <Pathfinding/Pathfinding.h>
#include <ECS/Entity/Entity.hpp>

struct EditorData
{
    BattleMap         battle_map;
    Camera            camera;
    Gizmo             gizmo;

    Registry          registry;
    Animator          animator;
    Pathfinding       pathfinding;

    sf::RenderWindow& window;

    EditorData(sf::RenderWindow& window)
    : window(window)
    ,camera(static_cast<sf::Vector2f>(window.getSize()), common::view)
    , animator(animator)
    , gizmo(registry)
    {
    }
};