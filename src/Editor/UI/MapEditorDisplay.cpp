#include "MapEditorDisplay.hpp"

#include <imgui.h>
#include <Common/IconText.h>
#include <SFML/Graphics.hpp>
#include "ToolsState.hpp"

MapEditorDisplay::MapEditorDisplay(sf::RenderWindow& window, BattleMap& battle_map)
    : battle_map(battle_map)
    , m_selected_tile_id(0)
    , is_brash(false)
{
}

void MapEditorDisplay::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::MAP), " Map editor"));

    renderLayersSection();
    renderTilesSection();

    ImGui::End();
}

void MapEditorDisplay::update(const float& delta_time)
{
    battle_map.setTileId(m_selected_tile_id);
}

void MapEditorDisplay::renderLayersSection()
{
    if (ImGui::CollapsingHeader(SET_ICON_TEXT((Icon::STACK_FILES), "Layers")))
    {
        ImGui::Text("Current layer: %d", battle_map.getCurrentLayerID());

        renderTilesetSelector();
        renderLayerControls();
    }
}

void MapEditorDisplay::renderTilesetSelector()
{
    static std::vector<const char*> items = { "Tileset1.png", "Tileset2.png", "Tileset3.png" };
    static int current_item = 0;
    static bool show_tileset_selector = false;

    if (ImGui::Button(SET_ICON_TEXT((Icon::ADD_FILES), "Add")))
        show_tileset_selector = true;

    if (show_tileset_selector)
        ImGui::OpenPopup("Select Tileset");

    if (ImGui::BeginPopupModal("Select Tileset", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Combo("Select Item", &current_item, items.data(), items.size());
        ImGui::SameLine();

        if (ImGui::Button("OK"))
        {
            battle_map.addLayer(items.at(current_item));
            show_tileset_selector = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            show_tileset_selector = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button(SET_ICON_TEXT((Icon::REMOVE_FILES), " Remove")))
        battle_map.removeLayer();

    ImGui::Separator();
}

void MapEditorDisplay::renderLayerControls()
{
    ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("FrameSelector", ImVec2(content_region_avail.x, 100));

    if (ImGui::BeginTable("LayersTable", 3))
    {
        ImGui::TableSetupColumn(0, ImGuiTableColumnFlags_WidthFixed, 25.0f);
        const auto& layers = battle_map.getLayers();

        for (size_t i = 0; i < layers.size(); ++i)
        {
            Layer* layer = layers[i].get();
            std::string default_layer_name = " Layer " + std::to_string(i);
            std::string display_layer_name = SET_ICON_TEXT((Icon::EMPTY_FILES), (layer->layer_name.empty() ? default_layer_name : layer->layer_name));

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(static_cast<int>(i));
            ImGui::Checkbox("##Visible", &layer->visible);
            ImGui::PopID();

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Selectable(display_layer_name.c_str(), battle_map.getCurrentLayerID() == i))
                battle_map.setCurrentLayerID(i);

            ImGui::TableSetColumnIndex(2);
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button(SET_ICON(Icon::EDIT)))
                ImGui::OpenPopup("Rename Layer");

            if (ImGui::BeginPopup("Rename Layer"))
            {
                static char layer_name_buffer[128] = "";
                strcpy(layer_name_buffer, layer->layer_name.empty() ? default_layer_name.c_str() : layer->layer_name.c_str());

                if (ImGui::InputText("##LayerName", layer_name_buffer, sizeof(layer_name_buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    layer->layer_name = layer_name_buffer;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

void MapEditorDisplay::renderTilesSection()
{
    if (ImGui::CollapsingHeader(SET_ICON_TEXT((Icon::FOUR_QUADS), "Tiles")))
        renderTilesetTable();
}

void MapEditorDisplay::renderTilesetTable()
{
    static int value = battle_map.getTileSize();
    const  int min_value = 8;
    const  int max_value = 128;

    ImGui::SliderInt("Scale", &value, min_value, max_value);
    ImGui::Separator();

    sf::Texture& tileset_Texture = battle_map.getTilesetTexture();
    int tileset_cols = battle_map.getSheetWidth();
    int tileset_rows = battle_map.getSheetHeight();

    ImVec2 scale_factor = ImVec2(value, value);

    ImTextureID tileset_texture_id = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle();

    if (ImGui::BeginTable("TilesetTable", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
    {
        for (int row = 0; row < tileset_rows; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < tileset_cols; col++)
            {
                ImGui::TableNextColumn();
                ImGui::PushID(row * tileset_cols + col);

                ImVec2 uv0 = ImVec2(col / (float)tileset_cols, row / (float)tileset_rows);
                ImVec2 uv1 = ImVec2((col + 1) / (float)tileset_cols, (row + 1) / (float)tileset_rows);

                int current_id = row * tileset_cols + col;
                bool selected = m_selected_tile_id == current_id;

                if (selected)
                {
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
                }

                if (ImGui::ImageButton("", tileset_texture_id, scale_factor, uv0, uv1, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
                {
                    m_selected_tile_id = row * tileset_cols + col;
                    battle_map.setSelectedTile(m_selected_tile_id);
                }

                if (selected)
                {
                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();
                }
                ImGui::PopID();
            }
        }
        ImGui::EndTable();
    }
}
