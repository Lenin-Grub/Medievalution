#pragma once
#include "State.hpp"
#include "LoadingState.hpp"
#include "SettingsState.hpp"
#include "BattleState.hpp"

class StateMachine;

class MenuState final 
    : public State
{
public:
    MenuState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

    void init()                                    override;
    void onDeactivate ()                           override;
    void onActivate   ()                           override;
    void updateEvents ()                           override;
    void updateImGui  ()                           override;
    void update (const float& dtime)               override;
    void draw (sf::RenderTarget* target = nullptr) override;

    void setBackground();

private:
    sf::Texture        background;
    sf::Sprite         sprite;
    sf::RectangleShape shape;
};
