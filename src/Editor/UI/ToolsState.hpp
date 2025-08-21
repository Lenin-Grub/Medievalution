#pragma once
#include <string>

enum class ToolType
{
    None,
    Select,
    Translate,
    Scale,
    Rotate,
    Brush,
    Bucket,
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

private:
    ToolsState() : active_tool(ToolType::None) {}
    ~ToolsState() = default;

    ToolType active_tool;
};
