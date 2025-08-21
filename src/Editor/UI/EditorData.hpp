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

    int selected_tile_id;
    bool is_brash;

    EditorData(sf::RenderWindow& window)
    : window(window)
    , camera(static_cast<sf::Vector2f>(window.getSize()), common::view)
    , animator(animator)
    , gizmo(registry)
    , is_brash(false)
    , selected_tile_id(0)
    {
    }
};