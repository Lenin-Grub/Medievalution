#include "../stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, m_board{ 32,sf::Vector2i(50, 50) }
, m_sprite_sheet{ "tileset.png", 32 }
, m_selected_tile_id { 0 }
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaulatView();
    pathfinding.initNodes(50, 50);
    m_sprite_sheet.import(m_board);
    m_board.initBoard();

    texture = ResourceLoader::instance().getTexture("Skeleton_archer.png");
    sprite.setTexture(texture);

    int a = 0;
    animator = std::make_unique<Animator>(sprite);
    animator->addFrame(sf::IntRect(a, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->addFrame(sf::IntRect(a += 128, 0, 128, 128));
    animator->setFrameTime(0.5f);
    animator->pause();

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
        state_machine.lastState();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
        m_sprite_sheet.addTileId(m_selected_tile_id, common::mouse_pos_view);

    if (!ImGui::GetIO().WantCaptureMouse)
        pathfinding.handleInput();

    data.camera.scroll();
    data.camera.zoom();
}

void BattleState::updateImGui()
{
#pragma region Exit
    ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button(Localization::getInstance().get("T_exit").c_str(), ImVec2(120, 0)))
    {
        state_machine.lastState();
    }
    ImGui::End();
#pragma endregion

#pragma region Editor
    {
        ImGui::Begin((ICON_MAP "Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        // Create a child window with scrolling
        static int value = m_sprite_sheet.getTileSize(); // Initial scale value
        const int  minValue = 8;                         // Minimum scale value
        const int  maxValue = 64;                        // Maximum scale value

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        sf::Texture& tileset_Texture = m_sprite_sheet.getTilesetTexture();
        //int cols = 4;
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
    }
#pragma endregion 

#pragma region Metrics
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    ImGui::End();
#pragma endregion

#pragma  region Animator
    {
        int selcted_id;
        ImGui::Begin(ICON_INSTAGRAM " Animation", nullptr);
        ImGui::Image(sprite, sf::Vector2f(256, 256));
        ImGui::Separator();
        auto ft = animator->getFrameTime();
        auto cf = animator->getCurrentFrame();
        auto pl = animator->isPlayed();
        ImGui::SliderFloat("time per frame", &ft, 0.0f, 1.0f);
        ImGui::SliderInt("frame", &cf, 0, animator->getFrames().size() - 1);
        ImGui::Checkbox("play", &pl);
        animator->setFrameTime(ft);
        animator->setCurrentFrame(cf);
        animator->play(pl);

        // Create a child window with scrolling
        static int value    = 128; // Initial scale value
        const int  minValue = 32;  // Minimum scale value
        const int  maxValue = 384; // Maximum scale value

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        ImVec2 scale_factor = ImVec2(value, value);
        int tileset_cols = std::round(texture.getSize().x / 128);
        int tileset_rows = std::round(texture.getSize().y / 128);

        ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)texture.getNativeHandle(); // Cast the texture ID to ImTextureID

        if (ImGui::BeginTable("Animation Table", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
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

                    if (ImGui::ImageButton((ImTextureID)tilesetTextureId, scale_factor, uv0, uv1, 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
                    {
                        selcted_id = row * tileset_cols + col;
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
    ImGui::End();
    }
#pragma endregion
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
    animator->update(0.1f);
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