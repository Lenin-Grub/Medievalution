#include "../stdafx.h"
#include "State.hpp"

State::State(StateData& data, StateMachine& machine, sf::RenderWindow &window, const bool replace)
: data{ data }
, state_machine{ machine }
, window{ window }
, m_replacing{ replace }
{

}

std::unique_ptr<State> State::next()
{
	return std::move( m_next );
}

bool State::isReplacing() const
{
	return m_replacing;
}

void State::updateMousePositions(sf::View* view)
{
	core::mouse_pos_screen	= sf::Mouse::getPosition();
	core::mouse_pos_window	= sf::Mouse::getPosition(window);
	core::mouse_pos_view	= window.mapPixelToCoords(sf::Mouse::getPosition(window));
}