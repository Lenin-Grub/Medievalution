#pragma once
#include "../State/State.hpp"
#include "../SettingsState/SettingsState.hpp"
#include "../StateMachine/StateMachine.hpp"


#include <World/WorldMap/WorldMap.h>
#include "Pathfinding/Pathfinding.h"
#include "ECS/Entity/Entity.hpp"


class StateMachine;

class GameState final 
    : public State
{
public:
    GameState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

    void init()                                   override;
    void onDeactivate()                           override;
    void onActivate()                             override;
    void updateEvents()                           override;
    void updateImGui()                            override;
    void update(const float& dtime)               override;
    void draw(sf::RenderTarget* target = nullptr) override;

private:
    void endView(sf::RenderTarget* target);
    void beginView(sf::RenderTarget*& target);

    // Use for ImGui
private:
    void renderMenu();
    void renderHelp();
    void renderNodesTree();

private:
    sf::Text           province_name;
    sf::Sprite         sprite;
    sf::Texture        texture;
    std::stringstream  cords;
    
    WorldMap           world_map;
    
    bool               is_loaded;

    Pathfinding        pathfinding;
    Node*              firstNode;
    Node*              secondNode;
    sf::Vector2f       position;


   std::unordered_map<const Node*, std::string> node_names;
   std::unordered_map<const Node*, int>         nodeIDs;
   char                                         search_buffer[128] = "";
};
