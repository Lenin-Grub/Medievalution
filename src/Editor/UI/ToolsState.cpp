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

bool ToolsState::isBrushActive() const
{
    return active_tool == ToolType::Brush;
}

bool ToolsState::isSelectActive() const
{
    return active_tool == ToolType::Select;
}

bool ToolsState::isTranslateActive() const
{
    return active_tool == ToolType::Translate;
}

bool ToolsState::isScaleActive() const
{
    return active_tool == ToolType::Scale;
}

bool ToolsState::isRotateActive() const
{
    return active_tool == ToolType::Rotate;
}
