#include "stdafx.h"
#include "BattleState.hpp"
#include "../StateMachine/StateMachine.hpp"

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
    editor.init();

    texture = ResourceLoader::instance().getTexture("Spearman.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    animator.setFrameTime(0.5f);
    animator.pause();

    ///-------------
    auto entity = entity_manager.createEntity();
    entity_manager.addComponent<Component_Position>(entity, sf::Vector2f(0.0f, 0.0f));
    entity_manager.addComponent<Component_Velocity>(entity, sf::Vector2f(0.0f, 0.0f));
    entity_manager.addComponent<Component_Sprite>(entity, sprite);
    entity_manager.addComponent<Control>(entity);
    entity_manager.addComponent<Control>(entity);
    entity_manager.setSprite(entity, "Spearman.png");

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

    data.camera.scroll();
    data.camera.zoom();
}

void BattleState::updateImGui() 
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    renderMetrics();
    renderEditor();
    renderAnimator();
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    entity_manager.update(1.0f / 60.0f, animator);
    animator.update(0.1f);
    data.camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    beginView(target);

    editor.draw(*target, sf::RenderStates::Default);
    entity_manager.draw(window);

    endView(target);
    ImGui::SFML::Render(window);
}


void BattleState::endView(sf::RenderTarget* target)
{
    target->setView(window.getDefaultView());
    target->setView(common::view);
}

void BattleState::beginView(sf::RenderTarget*& target)
{
    if (!target)
        target = &window;
    target->setView(common::view);
}


void BattleState::renderEditor()
{
    ImGui::Begin((ICON::getStr(Icon::MAP) + " Editor").c_str(), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    renderLayersSection();
    renderTilesSection();

    ImGui::End();
}

void BattleState::renderLayersSection()
{
    if (ImGui::CollapsingHeader((std::string(ICON::getStr(Icon::STACK_FILES)) + " Layers").c_str()))
    {
        ImGui::Text("Current layer: %d", editor.getCurrentLayer());

        renderTilesetSelector();
        renderLayerControls();
    }
}

void BattleState::renderTilesetSelector()
{
    static std::vector<const char*> items = { "Tileset1.png", "Tileset2.png" };
    static int current_item = 0;
    static bool show_tileset_selector = false;

    if (ImGui::Button((ICON::getStr(Icon::ADD_FILES) + " Add").c_str()))
    {
        show_tileset_selector = true;
    }

    if (show_tileset_selector)
    {
        ImGui::OpenPopup("Select Tileset");
    }

    if (ImGui::BeginPopupModal("Select Tileset", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Combo("Select Item", &current_item, items.data(), items.size());
        ImGui::SameLine();

        if (ImGui::Button("OK"))
        {
            editor.addLayer(items.at(current_item));
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

    if (ImGui::Button((ICON::getStr(Icon::REMOVE_FILES) + " Remove").c_str()))
    {
        editor.removeLayer();
    }

    ImGui::Separator();
}

void BattleState::renderLayerControls()
{
    ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("FrameSelector", ImVec2(content_region_avail.x, 100));

    if (ImGui::BeginTable("LayersTable", 3))
    {
        ImGui::TableSetupColumn(0, ImGuiTableColumnFlags_WidthFixed, 25.0f);
        const auto& layers = editor.getLayers();

        for (size_t i = 0; i < layers.size(); ++i)
        {
            Layer* layer = layers[i].get();
            std::string default_layer_name = " Layer " + std::to_string(i);
            std::string display_layer_name = ICON::getStr(Icon::EMPTY_FILES).c_str() + (layer->name.empty() ? default_layer_name : layer->name);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(static_cast<int>(i));
            ImGui::Checkbox("##Visible", &layer->visible);
            ImGui::PopID();

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Selectable(display_layer_name.c_str(), editor.getCurrentLayer() == i))
            {
                editor.setCurrentLayer(i);
            }

            ImGui::TableSetColumnIndex(2);
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button((ICON::getStr(Icon::EDIT)).c_str()))
            {
                ImGui::OpenPopup("Rename Layer");
            }

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

void BattleState::renderTilesSection()
{
    if (ImGui::CollapsingHeader((ICON::getStr(Icon::FOUR_QUADS) + " Tiles").c_str()))
    {
        static int value = editor.getTileSize();
        const int min_value = 8;
        const int max_value = 64;

        ImGui::SliderInt("Scale", &value, min_value, max_value);
        ImGui::Separator();

        renderTilesetTable();
    }
}

void BattleState::renderTilesetTable()
{
    sf::Texture& tileset_Texture = editor.getTilesetTexture();
    int tileset_cols = editor.getSheetWidth();
    int tileset_rows = editor.getSheetHeight();

    ImTextureID tileset_texture_id = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle();
    ImVec2 scale_factor = ImVec2(editor.getTileSize(), editor.getTileSize());

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


void BattleState::renderMetrics() 
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2#", nullptr, ImGuiWindowFlags_NoDecoration       | 
                                 ImGuiWindowFlags_AlwaysAutoResize   | 
                                 ImGuiWindowFlags_NoFocusOnAppearing | 
                                 ImGuiWindowFlags_NoNav);

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());

    ImGui::Columns(4, "table_columns");
    ImGui::Separator();

    ImGui::Text("Coordinates"); ImGui::SameLine();
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
    ImGui::Button("X"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%f", common::mouse_pos_view.x);
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    ImGui::Button("Y"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%f", common::mouse_pos_view.y);
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(23, 68, 210));
    ImGui::Button("Z"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%f", 1);

    ImGui::Columns(1);
    ImGui::End();
}


void BattleState::renderAnimator()
{
    ImGui::Begin((ICON::getStr(Icon::INSTAGRAM) + " Animation").c_str(), nullptr);

    renderSpriteSelector();
    renderAnimationControls();
    renderFrameControls();
    renderSpriteSheet();

    ImGui::End();
}

void BattleState::renderSpriteSelector()
{
    if (ImGui::Combo("Select entity", &current_item, items.data(), items.size()))
    {
        std::string selected_item = items.at(current_item);
        auto file = selected_item;
        selected_item += ".png";
        animator.init(selected_item.c_str());


        if (animator.loadAllAnimations(file))
        {
            auto size = animator.findAnimation(animation_name).size();
            for (size_t i = 0; i < size; i++)
            {
                animator.addFrame(animator.findAnimation(animation_name).at(i));
            }
        }
        else
        {
            LOG_ERROR("Fail load");
        }
    }

    ImGui::Image(sprite, sf::Vector2f(256, 256));
    ImGui::Separator();
}

void BattleState::renderAnimationControls()
{
    std::string selected_item = items.at(current_item);
   
    renderAnimationCombo();

    if (ImGui::Button((ICON::getStr(Icon::ADD_FILES) + " Add animation").c_str()))
    {
        ImGui::OpenPopup("Add Animation");
    }

    if (ImGui::BeginPopupModal("Add Animation", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("New Animation", animation_name, IM_ARRAYSIZE(animation_name));

        if (ImGui::Button((ICON::getStr(Icon::SAVE) + "Save").c_str()))
        {
            animator.addAnimation(animation_name);
            animator.saveAnimation(selected_item, animation_name);
            ImGui::CloseCurrentPopup();
            memset(animation_name, 0, sizeof(animation_name));
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            memset(animation_name, 0, sizeof(animation_name));
        }

        ImGui::EndPopup();
    }
}

void BattleState::renderAnimationCombo()
{
    std::vector<std::string> animationNames = animator.getAnimationNames();
    std::vector<const char*> animationNamesCStr;

    for (const auto& name : animationNames)
    {
        animationNamesCStr.push_back(name.c_str());
    }

    static int selectedAnimationIndex = 0;
    if (ImGui::Combo("Select animation", &selectedAnimationIndex, animationNamesCStr.data(), animationNamesCStr.size()))
    {
        animator.removeAllFrames();
        std::vector<sf::IntRect> frames = animator.findAnimation(animationNamesCStr.at(selectedAnimationIndex));
        for (const auto& frame : frames)
        {
            animator.addFrame(frame);
        }
    }
}

void BattleState::renderFrameControls()
{
    auto frame_time    = animator.getFrameTime();
    auto current_frame = animator.getCurrentFrame();
    auto is_played     = animator.isPlayed();

    ImGui::Separator();

    ImGui::Text("Current frame: %d", animator.getCurrentFrame());
    ImGui::SliderFloat("Time per frame", &frame_time, 0.0f, 1.0f);
    ImGui::SliderInt("Frame", &current_frame, 0, animator.getFrames().empty() ? 0 : animator.getFrames().size() - 1);

    renderPlaybackButtons(current_frame, is_played);

    animator.setFrameTime(frame_time);
    animator.setCurrentFrame(current_frame);
    animator.play(is_played);

    renderAddRemoveFrameButtons();
    renderFrameSelector();
}

void BattleState::renderPlaybackButtons(int& current_frame, bool& is_payed)
{
    if (ImGui::Button((ICON::getStr(Icon::BEGIN) + "##Begin").c_str()))
    {
        current_frame = animator.getFirstFrame();
        animator.setCurrentFrame(current_frame);
    }

    ImGui::SameLine();

    if (ImGui::Button((ICON::getStr(Icon::PREV) + "##Prev").c_str()))
    {
        current_frame = animator.getPrevFrame();
        animator.setCurrentFrame(current_frame);
    }

    ImGui::SameLine();

    if (ImGui::Button(is_payed ? (ICON::getStr(Icon::PLAY) + "##Play").c_str() : (ICON::getStr(Icon::PAUSE) + "##Pause").c_str()))
    {
        is_payed = !is_payed;
    }

    ImGui::SameLine();

    if (ImGui::Button((ICON::getStr(Icon::NEXT) + "##Next").c_str()))
    {
        current_frame = animator.getNextFrame();
        animator.setCurrentFrame(current_frame);
    }

    ImGui::SameLine();

    if (ImGui::Button((ICON::getStr(Icon::END) + "##End").c_str()))
    {
        current_frame = animator.getLastFrame();
        animator.setCurrentFrame(current_frame);
    }

    ImGui::Separator();
}

void BattleState::renderAddRemoveFrameButtons()
{
    static int size  = 64;
    int tileset_cols = std::round(texture.getSize().x / size);
    int tileset_rows = std::round(texture.getSize().y / size);

    if (ImGui::Button((ICON::getStr(Icon::ADD_FILES) + "Add").c_str()))
    {
        int tile_x = m_animator_tile_selected_id % tileset_cols;
        int tile_y = m_animator_tile_selected_id / tileset_cols;
        sf::IntRect rect(tile_x * size, tile_y * size, size, size);
        animator.addFrame(rect);
    }

    ImGui::SameLine();

    if (ImGui::Button((ICON::getStr(Icon::REMOVE_FILES) + "Remove").c_str()))
    {
        animator.removeFrame(animator.getCurrentFrame());
    }
}

void BattleState::renderFrameSelector()
{
    if (ImGui::CollapsingHeader("Frames"))
    {
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("FrameSelector", ImVec2(contentRegionAvail.x, 150));

        const auto& frames = animator.getFrames();
        for (size_t i = 0; i < frames.size(); ++i)
        {
            std::string frame_name = ((ICON::getStr(Icon::EMPTY_FILES) + "Frame ").c_str()) + std::to_string(i);
            if (ImGui::Selectable(frame_name.c_str(), animator.getCurrentFrame() == i))
            {
                animator.setCurrentFrame(i);
            }
        }
        ImGui::EndChild();
    }
}

void BattleState::renderSpriteSheet()
{
    static int size     = 64;
    static int value    = 96;
    const int  minValue = 32;
    const int  maxValue = 128;

    ImVec2 scale_factor = ImVec2(value, value);
    int tileset_cols = std::round(texture.getSize().x / size);
    int tileset_rows = std::round(texture.getSize().y / size);

    if (ImGui::CollapsingHeader("Sprite Sheet"))
    {
        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)sprite.getTexture()->getNativeHandle();

        if (ImGui::BeginTable("Animation Table", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY))
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
                    bool selected = m_animator_tile_selected_id == current_id;

                    if (selected)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 6.0f);
                    }

                    if (ImGui::ImageButton("1", tilesetTextureId, scale_factor, uv0, uv1, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
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
    }
}
