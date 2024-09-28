#pragma once
#include "State.hpp"
#include "GameState.hpp"

class StateMachine;

class LoadingState final 
    : public State
{
public:
    LoadingState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

    void onDeactivate()                           override;
    void onActivate  ()                           override;
    void updateEvents()                           override;
    void updateImGui ()                           override;
    void update(const float& dtime)               override;
    void draw(sf::RenderTarget* target = nullptr) override;

private:
    void setBackground();

    sf::Texture        background;
    sf::Sprite         sprite;
    sf::RectangleShape shape;
    sf::Color          alpha;
    sf::Text           text;
    sf::Font           font;
};