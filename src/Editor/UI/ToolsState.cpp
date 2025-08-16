#include "ToolsState.hpp"

void ToolsState::setActiveTool(ToolType tool)
{
    active_tool = tool;
}

ToolType ToolsState::getActiveTool() const
{
    return active_tool;
}

bool ToolsState::isToolActive(ToolType tool) const
{
    return active_tool == tool;
}
