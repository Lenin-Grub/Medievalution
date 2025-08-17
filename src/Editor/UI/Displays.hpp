#pragma once
#include <SFML/Graphics.hpp>
#include <Camera/Camera.h>
#include "LogDisplay.hpp"
#include "SceneHierarhyDisplay.hpp"
#include "SceneDisplay.hpp"
#include "AnimationDisplay.hpp"
#include "MenuDisplay.hpp"
#include "MapEditorDisplay.hpp"
#include "ObjectDetailsDisplay.hpp"
#include "InspectorDisplay.hpp"
#include "AssetsDisplay.hpp"
#include <ECS/Entity/Entity.hpp>

class Displays
{
public:
	Displays(sf::RenderWindow& window, BattleMap& battle_map, Registry& registry, Gizmo& gizmo);
	~Displays() = default;

    bool initDockSpace();
    bool create();
    void draw();
    void update(const float& delta_time);

    void metrics();

    void initDockingLayout();

    sf::Vector2f world_mouse_pos;
private:
    sf::RenderWindow&     window;
    sf::Clock             clock;
    sf::Vector2f          window_size;
    sf::RenderTexture     render_texture;

    LogDisplay            display_log;
    SceneHierarchyDisplay display_scene_hierarchy;
    SceneDisplay          display_scene;
    AnimationDisplay      display_animation;
    MenuDisplay           display_menu;
    MapEditorDisplay      display_map_editor;
    ObjectDetailsDisplay  display_object_details;
    InspectorDisplay      display_inspector;
    AssetsDisplay         display_assets;

    BattleMap&            battle_map;
    Gizmo&                gizmo;
};
