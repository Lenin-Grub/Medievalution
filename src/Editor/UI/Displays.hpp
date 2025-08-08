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
#include <ECS/Entity/Entity.hpp>

class Displays
{
public:
	Displays(sf::RenderWindow& window, BattleMap& battle_map, Registry& registry);
	~Displays() = default;

    bool initDockSpace();
    bool create();
    void draw();
    void update(const float& delta_time);

    void initDockingLayout();

private:
    sf::RenderWindow& window;
    sf::Clock         clock;
    sf::Vector2f      window_size;
    sf::RenderTexture render_texture;

    LogDisplay            display_log;
    SceneHierrarhyDisplay display_scene_hierrarhy;
    SceneDisplay          display_scene;
    AnimationDisplay      display_animation;
    MenuDisplay           display_menu;
    MapEditorDisplay      display_map_editor;
    ObjectDetailsDisplay  display_object_details;
    InspectorDisplay      display_inspector;

    BattleMap&             battle_map;
};