#include "stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, m_selected_tile_id (0)
, m_animator_tile_selected_id (0)
, animator(sprite)
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaulatView();
    pathfinding.initNodes(50, 50);
    editor.init();

    texture = ResourceLoader::instance().getTexture("Spearman.png");
    sprite.setTexture(texture);
    animator.setFrameTime(0.5f);
    animator.pause();

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
        editor.addTile(m_selected_tile_id, common::mouse_pos_view);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !ImGui::GetIO().WantCaptureMouse)
        editor.removeTile(m_selected_tile_id, common::mouse_pos_view);

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
    ImGui::Begin((ICON_MAP "Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    if (ImGui::CollapsingHeader((ICON_STACK_FILES "Layers")))
    {
        ImGui::Text("Current layer: %d", editor.getCurrentLayer());

        static std::vector<const char*> items = { "Tileset1.png", "Tileset2.png" };
        static int current_item = 0;

        ImGui::Combo("Select Item", &current_item, items.data(), items.size());

        if (ImGui::Button((ICON_ADD_FILES "Add")))
            editor.addLayer(items.at(current_item));

        ImGui::SameLine();

        if (ImGui::Button((ICON_REMOVE_FILES "Remove")))
            editor.removeLayer();

        ImGui::Separator();
        //______________________________________
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("FrameSelector", ImVec2(contentRegionAvail.x, 100));
        if (ImGui::BeginTable("LayersTable", 2)) 
        {
            const auto& layers = editor.getLayers();
            for (size_t i = 0; i < layers.size(); ++i)
            {
                Layer* layer = layers[i].get();
                std::string layerName = (ICON_EMPTY_FILES "Layer ") + std::to_string(i);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); // Первый столбец для метки слоя
                if (ImGui::Selectable(layerName.c_str(), editor.getCurrentLayer() == i))
                    editor.setCurrentLayer(i);

                ImGui::TableSetColumnIndex(1); // Второй столбец для чекбокса
                ImGui::Checkbox("Visible", &layer->visible);
            }
            ImGui::EndTable();
            ImGui::EndChild();
        }
    }

    //______________________________________
    if (ImGui::CollapsingHeader((ICON_FOUR_QUADS "Tiles")))
    {
        static int value = editor.getTileSize(); // Initial scale value
        const int minValue = 8;                  // Minimum scale value
        const int maxValue = 64;                 // Maximum scale value

        ImGui::SliderInt("Scale", &value, minValue, maxValue);
        ImGui::Separator();

        sf::Texture& tileset_Texture = editor.getTilesetTexture();
        int tileset_cols = editor.getSheetWidth();
        int tileset_rows = editor.getSheetHeight();

        ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle(); // Cast the texture ID to ImTextureID

        ImVec2 scale_factor = ImVec2(value, value);

        if (ImGui::BeginTable("TilesetTable", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
            for (int row = 0; row < tileset_rows; row++) {
                ImGui::TableNextRow();
                for (int col = 0; col < tileset_cols; col++) {
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

                    if (ImGui::ImageButton((ImTextureID)tilesetTextureId, scale_factor, uv0, uv1, 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1))) {
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
    }
    ImGui::End();

#pragma endregion 

#pragma region Metrics
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2#", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    
    ImGui::Columns(3, "table_columns");
    ImGui::Separator();

    ImGui::Text("Coordinates"); ImGui::SameLine();
    ImGui::NextColumn();
    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
    ImGui::Button("X"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%f", common::mouse_pos_view.x);
    ImGui::NextColumn();
    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Green);
    ImGui::Button("Y"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%f", common::mouse_pos_view.y);

    ImGui::Columns(1);
    ImGui::End();
#pragma endregion

#pragma  region Animator
    {
        ImGui::Begin(ICON_INSTAGRAM " Animation", nullptr);

        static std::vector<const char*> items = { "Spearman.png", "Archer.png" };
        static int current_item = 0;

        ImGui::Combo("Select sprite", &current_item, items.data(), items.size());

        if (ImGui::Button((ICON_ADD_FILES "Change")))
            animator.init(items.at(current_item));

        ImGui::Image(sprite, sf::Vector2f(256, 256));

        ImGui::Separator();

        auto ft = animator.getFrameTime();
        auto cf = animator.getCurrentFrame();
        auto pl = animator.isPlayed();

        static int value    = 96; // Initial scale value
        const int  minValue = 32;  // Minimum scale value
        const int  maxValue = 128; // Maximum scale value

        ImVec2 scale_factor = ImVec2(value, value);
        int tileset_cols = std::round(texture.getSize().x / 64);
        int tileset_rows = std::round(texture.getSize().y / 64);

        ImGui::Text("Current frame: %d", animator.getCurrentFrame());
        ImGui::SliderFloat("Time per frame", &ft, 0.0f, 1.0f);
        ImGui::SliderInt("Frame", &cf, 0, animator.getFrames().empty() ? 0 : animator.getFrames().size() - 1);
        ImGui::Checkbox("Play", &pl);

        animator.setFrameTime(ft);
        animator.setCurrentFrame(cf);
        animator.play(pl);

        if (ImGui::Button((ICON_ADD_FILES "Add")))
            {
                int tile_x = m_animator_tile_selected_id % tileset_cols;
                int tile_y = m_animator_tile_selected_id / tileset_cols;
                sf::IntRect rect(tile_x * 64, tile_y * 64, 64, 64);
                animator.addFrame(rect);
            }

        ImGui::SameLine();

        if (ImGui::Button((ICON_REMOVE_FILES "Remove")))
        {
            animator.removeFrmae(animator.getCurrentFrame());
        }
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("FrameSelector", ImVec2(contentRegionAvail.x, 150));
        const auto& frames = animator.getFrames();
        for (size_t i = 0; i < frames.size(); ++i)
        {
            auto m_anim = animator.getFrames().at(i);
            std::string frame_name = (ICON_EMPTY_FILES "Frame ") + std::to_string(i);
            if (ImGui::Selectable(frame_name.c_str(), animator.getCurrentFrame() == i))
                animator.setCurrentFrame(i);
        }
        ImGui::EndChild();

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

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

                    bool selected = m_animator_tile_selected_id == current_id;

                    if (selected)
                    {
                        // You can adjust the border color and width here
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
                    }

                    if (ImGui::ImageButton((ImTextureID)tilesetTextureId, scale_factor, uv0, uv1, 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
                    {
                        m_animator_tile_selected_id = row * tileset_cols + col;
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
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
    animator.update(0.1f);
    data.camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(common::view);
    
    target->draw(editor);
    pathfinding.draw(window);

    target->setView(window.getDefaultView());
    target->setView(common::view);

    ImGui::SFML::Render(window);
}