#pragma once
#include "State.h"
#include "StateManager.h"

class MainMenuState :
    public State
{
public:

    MainMenuState(StateData& state_data, StateMachine& state_manager);
    ~MainMenuState();

    bool init()                                         override;
    void updateEvents()                                 override;
    void updateImGui()                                  override;
    void update(const float& dtime)                     override;
    void render(sf::RenderTarget* target = nullptr)     override;

private:
    StateMachine& state_manager;
};

