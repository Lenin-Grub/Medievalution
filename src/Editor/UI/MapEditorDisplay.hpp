#pragma once

#include <UI/IDisplay.hpp>
#include <World/BattleMap/BattleMap.hpp>
#include "ToolsState.hpp"
#include "EditorData.hpp"

class MapEditorDisplay
    : public UI::IDisplay
{
public:
    MapEditorDisplay(sf::RenderWindow& window, EditorData& data);
    virtual ~MapEditorDisplay() = default;

    void draw() override;
    void update(const float& delta_time) override;

private:
    void renderLayersSection();
    void renderTilesetSelector();
    void renderLayerControls();
    void renderTilesSection();
    void renderTilesetTable();

private:
    int               m_selected_tile_id;
    BattleMap&        battle_map;

    sf::Sprite        sprite;
    sf::Texture       texture;

    bool is_brash;
};
