#include "../../stdafx.h"
#include "GUIBattleState.h"


void GUI::updateBattleStateImGui(BattleState& battleState, State& state)
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

#pragma region Exit
    ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button(Localization::getInstance().get("T_exit").c_str(), ImVec2(120, 0)))
    {
        state.state_machine.lastState();
    }
    ImGui::End();
#pragma endregion

#pragma region Editor
    ImGui::Begin((ICON_MAP "Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    if (ImGui::CollapsingHeader((ICON_STACK_FILES "Layers")))
    {
        ImGui::Text("Current layer: %d", battleState.editor.getCurrentLayer());

        static std::vector<const char*> items = { "Tileset1.png", "Tileset2.png", "Door1.png", "Door2.png", "DoubleDoor1.png", "DoubleDoor2.png" };
        static int  current_item = 0;
        static bool show_tileset_selector = false;

        if (ImGui::Button((ICON_ADD_FILES "Add")))
            show_tileset_selector = true;

        if (show_tileset_selector)
            ImGui::OpenPopup("Select Tileset");

        if (ImGui::BeginPopupModal("Select Tileset", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Combo("Select Item", &current_item, items.data(), items.size());

            ImGui::SameLine();

            if (ImGui::Button("OK")) {
                battleState.editor.addLayer(items.at(current_item));
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

        if (ImGui::Button((ICON_REMOVE_FILES "Remove")))
            battleState.editor.removeLayer();

        ImGui::Separator();
        //______________________________________
        ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("FrameSelector", ImVec2(content_region_avail.x, 100));
        if (ImGui::BeginTable("LayersTable", 3))
        {
            ImGui::TableSetupColumn(0, ImGuiTableColumnFlags_WidthFixed, 25.0f);
            const auto& layers = battleState.editor.getLayers();
            for (size_t i = 0; i < layers.size(); ++i)
            {
                Layer* layer = layers[i].get();
                std::string default_layer_name = "Layer " + std::to_string(i);
                std::string display_layer_name = ICON_EMPTY_FILES + (layer->name.empty() ? default_layer_name : layer->name);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::PushID(static_cast<int>(i));
                ImGui::Checkbox("##Visible", &layer->visible);
                ImGui::PopID();

                ImGui::TableSetColumnIndex(1);
                if (ImGui::Selectable(display_layer_name.c_str(), battleState.editor.getCurrentLayer() == i))
                    battleState.editor.setCurrentLayer(i);

                ImGui::TableSetColumnIndex(2);
                ImGui::PushID(static_cast<int>(i));
                if (ImGui::Button(ICON_EDIT))
                    ImGui::OpenPopup("Rename Layer");

                if (ImGui::BeginPopup("Rename Layer"))
                {
                    static char layer_name_buffer[128] = "";
                    strcpy(layer_name_buffer, layer->name.empty() ? default_layer_name.c_str() : layer->name.c_str());
                    if (ImGui::InputText("##LayerName", layer_name_buffer, sizeof(layer_name_buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        layer->name = layer_name_buffer;
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

    //______________________________________
    if (ImGui::CollapsingHeader((ICON_FOUR_QUADS "Tiles")))
    {
        static int value = battleState.editor.getTileSize(); // Initial scale value
        const int min_value = 8;                  // Minimum scale value
        const int max_value = 64;                 // Maximum scale value

        ImGui::SliderInt("Scale", &value, min_value, max_value);
        ImGui::Separator();

        sf::Texture& tileset_Texture = battleState.editor.getTilesetTexture();
        int tileset_cols = battleState.editor.getSheetWidth();
        int tileset_rows = battleState.editor.getSheetHeight();

        ImTextureID tileset_texture_id = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle(); // Cast the texture ID to ImTextureID

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
                    bool selected = battleState.m_selected_tile_id == current_id;

                    if (selected)
                    {
                        // You can adjust the border color and width here
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
                    }

                    if (ImGui::ImageButton("", (ImTextureID)tileset_texture_id, scale_factor, uv0, uv1, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1))) {
                        battleState.m_selected_tile_id = row * tileset_cols + col;
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

        if (ImGui::Combo("Select sprite", &current_item, items.data(), items.size()))
            battleState.animator.init(items.at(current_item));

        ImGui::Image(battleState.sprite, sf::Vector2f(256, 256));

        ImGui::Separator();

        auto ft = battleState.animator.getFrameTime();
        auto cf = battleState.animator.getCurrentFrame();
        auto pl = battleState.animator.isPlayed();

        static int size = 64;  // Tile    scale value
        static int value = 96;  // Initial scale value
        const int  minValue = 32;  // Minimum scale value
        const int  maxValue = 128; // Maximum scale value

        ImVec2 scale_factor = ImVec2(value, value);
        int tileset_cols = std::round(battleState.texture.getSize().x / size);
        int tileset_rows = std::round(battleState.texture.getSize().y / size);

        static bool m_show_popup = false;

        if (ImGui::Button((ICON_ADD_FILES "Add animation")))
            m_show_popup = true;

        if (m_show_popup)
            ImGui::OpenPopup("Add Animation Popup");

        if (ImGui::BeginPopup("Add Animation Popup"))
        {
            static char animationName[64] = "";

            ImGui::InputText("Animation Name", animationName, IM_ARRAYSIZE(animationName));

            if (ImGui::Button("Add"))
            {
                battleState.animator.addAnimation(animationName);
                m_show_popup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_show_popup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::Text("Current frame: %d", battleState.animator.getCurrentFrame());
        ImGui::SliderFloat("Time per frame", &ft, 0.0f, 1.0f);
        ImGui::SliderInt("Frame", &cf, 0, battleState.animator.getFrames().empty() ? 0 : battleState.animator.getFrames().size() - 1);

        if (ImGui::Button((ICON_BEGIN "##Begin")))
        {
            cf = battleState.animator.getFirstFrame();
            battleState.animator.setCurrentFrame(cf);
        }
        ImGui::SameLine();
        if (ImGui::Button((ICON_PREV "##Prev")))
        {
            cf = battleState.animator.getPrevFrame();
            battleState.animator.setCurrentFrame(cf);
        }
        ImGui::SameLine();
        if (ImGui::Button(pl ? (ICON_PLAY"##Play") : (ICON_PAUSE"##Pause")))
        {
            pl = !pl;
        }
        ImGui::SameLine();
        if (ImGui::Button((ICON_NEXT "##Next")))
        {
            cf = battleState.animator.getNextFrame();
            battleState.animator.setCurrentFrame(cf);
        }
        ImGui::SameLine();
        if (ImGui::Button((ICON_END "##End")))
        {
            cf = battleState.animator.getLastFrame();
            battleState.animator.setCurrentFrame(cf);
        }

        battleState.animator.setFrameTime(ft);
        battleState.animator.setCurrentFrame(cf);
        battleState.animator.play(pl);

        if (ImGui::Button((ICON_ADD_FILES "Add")))
        {
            int tile_x = battleState.m_animator_tile_selected_id % tileset_cols;
            int tile_y = battleState.m_animator_tile_selected_id / tileset_cols;
            sf::IntRect rect(tile_x * size, tile_y * size, size, size);
            battleState.animator.addFrame(rect);
        }

        ImGui::SameLine();

        if (ImGui::Button((ICON_REMOVE_FILES "Remove")))
            battleState.animator.removeFrame(battleState.animator.getCurrentFrame());
        if (ImGui::CollapsingHeader("Frames"))
        {
            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
            ImGui::BeginChild("FrameSelector", ImVec2(contentRegionAvail.x, 150));
            const auto& frames = battleState.animator.getFrames();
            for (size_t i = 0; i < frames.size(); ++i)
            {
                auto m_anim = battleState.animator.getFrames().at(i);
                std::string frame_name = (ICON_EMPTY_FILES "Frame ") + std::to_string(i);
                if (ImGui::Selectable(frame_name.c_str(), battleState.animator.getCurrentFrame() == i))
                    battleState.animator.setCurrentFrame(i);
            }
            ImGui::EndChild();
        }

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)battleState.sprite.getTexture()->getNativeHandle(); // Cast the texture ID to ImTextureID

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

                    bool selected = battleState.m_animator_tile_selected_id == current_id;

                    if (selected)
                    {
                        // You can adjust the border color and width here
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
                    }

                    if (ImGui::ImageButton("1", (ImTextureID)tilesetTextureId, scale_factor, uv0, uv1, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
                    {
                        battleState.m_animator_tile_selected_id = row * tileset_cols + col;
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
