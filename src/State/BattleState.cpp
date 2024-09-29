#include "../stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, camera()

{
    LOG_INFO("State Battle\t Init");
    pathfinding.initNodes(50, 50);
    try
    {
        m_sprite_sheet = { "Map/tileset.png", 32};
        m_sprite_sheet.import(m_board);
    }
    catch (const std::exception& ex)
    {
        if (dynamic_cast<const std::invalid_argument*>(&ex))
        {
            LOG_WARN("Error load tileset!");
        }
    }
    m_board.initBoard();
}

void BattleState::init()
{
}

void BattleState::onDeactivate()
{
    LOG_INFO("State Battle\t Deactivate");
}

void BattleState::onActivate()
{
    LOG_INFO("State Battle\t Activate");
}

void BattleState::updateEvents()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
    {
        m_sprite_sheet.addTileId(m_selected_tile_id, core::mouse_pos_view);
    }
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        pathfinding.handleInput();
    }
    camera.scroll();
    camera.zoom();
}

void BattleState::updateImGui()
{
    ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button(Localization::getInstance().getStringByKey("T_exit").c_str(), ImVec2(120, 0)))
    {
        state_machine.lastState();
    }
    ImGui::End();

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // Create a child window with scrolling
    static int value = 32; // Initial scale value
    const int  minValue = 8;  // Minimum scale value
    const int  maxValue = 64; // Maximum scale value

    ImGui::SliderInt("Scale", &value, minValue, maxValue);

    sf::Texture& tileset_Texture = m_sprite_sheet.getTilesetTexture();
    int tileset_cols = m_sprite_sheet.getSheetWidth();
    int tileset_rows = m_sprite_sheet.getSheetHeight();

    ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle(); // Cast the texture ID to ImTextureID

    ImVec2 scale_factor = ImVec2(value, value);

    if (ImGui::BeginTable("TilesetTable", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
    {
        for (int row = 0; row < tileset_rows; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < tileset_cols; col++)
            {
                ImGui::TableNextColumn();

                // Create a unique ID for the button using row and column indices
                ImGui::PushID(row * tileset_cols + col);

                ImVec2 uv0 = ImVec2(col / (float)tileset_cols, row / (float)tileset_rows);
                ImVec2 uv1 = ImVec2((col + 1) / (float)tileset_cols, (row + 1) / (float)tileset_rows);

                int current_id = row * tileset_cols + col;
                bool selected = m_selected_tile_id == current_id;

                if (selected)
                {
                    // You can adjust the border color and width here
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
                }

                if (ImGui::ImageButton((ImTextureID)tilesetTextureId, scale_factor, uv0, uv1, 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
                {
                    m_selected_tile_id = row * tileset_cols + col;
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
    ImGui::End();
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
    camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    window.clear();
    if (!target)
        target = &window;
    target->setView(core::view);

    target->draw(m_board);
    m_sprite_sheet.mergeTiles();
    target->draw(m_sprite_sheet);
    pathfinding.draw(window);

    target->setView(window.getDefaultView());
    target->setView(core::view);

    ImGui::SFML::Render(window);

    window.display();
}