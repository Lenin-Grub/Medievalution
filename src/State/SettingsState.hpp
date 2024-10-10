#pragma once
#include "State.hpp"

class StateMachine;

class SettingsState final 
    : public State
{
public:
    SettingsState (StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

    void init()                                            override;
    void onDeactivate ()                                   override;
    void onActivate   ()                                   override;
    void updateEvents ()                                   override;
    void updateImGui  ()                                   override;
    void update       (const float& dtime)                 override;
    void draw         (sf::RenderTarget* target = nullptr) override;

private:
    void setBackground ();

    int                        resolution_current_id;
    bool                       play_music;
    bool                       play_sound;

    sf::Texture                background;
    sf::Sprite                 sprite;
    sf::RectangleShape         shape;

    std::vector<sf::VideoMode> video_modes;
    std::string                string_resolution;
};
