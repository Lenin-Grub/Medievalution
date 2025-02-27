#pragma once

#include "../../World/Editor/Editor.hpp"
#include "../../Animation/Animation.hpp"
#include "../../Localisation/Localisation.hpp"
#include "../../Settings/WindowSettings.h"
#include "../StateMachine.hpp"
#include "../BattleState.hpp"

class BattleState;
class State;

namespace GUI 
{
    void updateBattleStateImGui(BattleState& battleState, State& state);
}