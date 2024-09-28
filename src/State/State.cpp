#include "../stdafx.h"
#include "State.hpp"

State::State (StateData& data, StateMachine& machine, sf::RenderWindow &window, const bool replace)
: data          { data }
, state_machine { machine }
, window        { window }
, is_replace    { replace }
{
    // Do nothing
}

std::unique_ptr<State> State::next()
{
    return std::move( next_state );
}

bool State::isReplacing() const
{
    return is_replace;
}

void State::updateMousePositions(sf::View* view)
{
    core::mouse_pos_screen	= sf::Mouse::getPosition();
    core::mouse_pos_window	= sf::Mouse::getPosition(window);
    core::mouse_pos_view	= window.mapPixelToCoords(sf::Mouse::getPosition(window));
}