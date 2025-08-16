#pragma once

#include <string>

enum class ToolType
{
    None,
    Brush,
    Select,
    Translate,
    Scale,
    Rotate
};

class ToolsState
{
public:
    static ToolsState& instance()
    {
        static ToolsState instance;
        return instance;
    }

    void setActiveTool(ToolType tool);
    ToolType getActiveTool() const;
    bool isToolActive(ToolType tool) const;
    bool isBrushActive() const;
    bool isSelectActive() const;
    bool isTranslateActive() const;
    bool isScaleActive() const;
    bool isRotateActive() const;

private:
    ToolsState() : active_tool(ToolType::None) {}
    ~ToolsState() = default;

    ToolType active_tool;
};
