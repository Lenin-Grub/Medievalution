#include "../stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaulatView();
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
    LOG_INFO("State Battle\t Init");
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
    if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        state_machine.lastState();
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
    {
        m_sprite_sheet.addTileId(m_selected_tile_id, common::mouse_pos_view);
    }
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        pathfinding.handleInput();
    }
    data.camera.scroll();
    data.camera.zoom();
}

void BattleState::updateImGui()
{
    ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button(Localization::getInstance().get("T_exit").c_str(), ImVec2(120, 0)))
    {
        state_machine.lastState();
    }
    ImGui::End();

    ImGui::Begin((ICON_MAP "Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // Create a child window with scrolling
    static int value = 32; // Initial scale value
    const int  minValue = 8;  // Minimum scale value
    const int  maxValue = 64; // Maximum scale value

    ImGui::SliderInt("Scale", &value, minValue, maxValue);

    sf::Texture& tileset_Texture = m_sprite_sheet.getTilesetTexture();
    int cols = 4;
    int tileset_cols = m_sprite_sheet.getSheetWidth();
    int tileset_rows = m_sprite_sheet.getSheetHeight();

    ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle(); // Cast the texture ID to ImTextureID

    ImVec2 scale_factor = ImVec2(value, value);

    if (ImGui::BeginTable("TilesetTable", cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
    {
        for (int row = 0; row < tileset_rows; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < cols; col++)
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
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
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

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
    data.camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(common::view);

    target->draw(m_board);
    m_sprite_sheet.mergeTiles();
    target->draw(m_sprite_sheet);
    pathfinding.draw(window);

    target->setView(window.getDefaultView());
    target->setView(common::view);

    ImGui::SFML::Render(window);
}