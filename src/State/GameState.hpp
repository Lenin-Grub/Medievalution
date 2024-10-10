#pragma once
#include "State.hpp"
#include "SettingsState.hpp"
#include "../World/WorldMap.h"

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
    sf::Text          province_name;
    sf::Sprite        sprite;
    sf::Texture       texture;
    std::stringstream cords;
    WorldMap          world_map;
    bool              is_loaded;
};
