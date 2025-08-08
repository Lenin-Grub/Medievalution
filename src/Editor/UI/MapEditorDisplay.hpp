#pragma once

#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include <Gizmos/Gizmos.h>

enum class ToolState
{
    None = 0,
    Translate,
    Rotate,
    Scale,
    Brush,
    Fill,
    Flip,
    Undo,
    Redo
};

class MapEditorDisplay
	: public UI::IDisplay
{
public:
	MapEditorDisplay(sf::RenderWindow& window, BattleMap& battle_map);
	virtual ~MapEditorDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

    void renderLayersSection();

    void renderTilesetSelector();

    void renderLayerControls();

    void renderTilesSection();

    void renderTilesetTable();

private:
    ToolState         tools;
    Gizmos            gizmos;
    BattleMap&        battle_map;

    sf::Sprite        sprite;
    sf::Texture       texture;

    int               m_selected_tile_id;
    bool              is_brash;
    bool              show_editor_window = false;
    bool              show_metrics_window = false;
    bool              show_animator_window = false;

};