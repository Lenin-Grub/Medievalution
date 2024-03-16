#pragma once
#include "State.h"
#include "StateManager.h"

class IntroState :
    public State
{
public:

    IntroState(StateData& state_data, StateMachine& state_manager);
    ~IntroState();

    bool init()                                         override;
    void updateEvents()                                 override;
    void updateImGui()                                  override;
    void update(const float& dtime)                     override;
    void render(sf::RenderTarget* target = nullptr)     override;

private:
    StateMachine& state_manager;
};

