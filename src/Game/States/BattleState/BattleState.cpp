#include "BattleState.hpp"
#include "../StateMachine/StateMachine.hpp"

#include <States/SettingsState/SettingsState.hpp>

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, m_selected_tile_id (0)
, animator(sprite)
, gizmos(window, sf::Vector2f(10,10))
, is_brash(false)
, tools (ToolState::None)
, animaton_editor (window)
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaultView();
    data.camera.reset();

    battle_map.init();

    texture = ResourceLoader::instance().getTexture("Spearman.png");
    pathfinding.initNodes(64, 64);

    UnitFactory factory(registry, texture);

    Entity spearman1 = factory.createSpearman(sf::Vector2f(0,0));
    Entity spearman2 = factory.createSpearman(sf::Vector2f(64, 64));
    Entity spearman3 = factory.createSpearman(sf::Vector2f(128, 64));
    Entity spearman4 = factory.createSpearman(sf::Vector2f(128, 128));
   
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

    if (is_brash)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
            battle_map.addTile(m_selected_tile_id, common::mouse_pos_view);


        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !ImGui::GetIO().WantCaptureMouse)
            battle_map.removeTile(common::mouse_pos_view);
    }

    PathfindingInputSystem::handleInput(pathfinding);

    data.camera.scroll();
    data.camera.zoom();

    gizmos.update();
}

void BattleState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem(SET_ICON_TEXT((Icon::TOOL), "Map editor")))
        {
            show_editor_window = true;
        }

        if (ImGui::MenuItem(SET_ICON_TEXT((Icon::INSTAGRAM), "Animation editor")))
        {
            show_animator_window = true;
        }

        if (ImGui::MenuItem(SET_ICON_TEXT((Icon::METRIC_UP), "Metric")))
        {
            show_metrics_window = true;
        }

        ImGui::Separator();
        ImGui::EndMenu();
    }

    renderTools();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);

    if (ImGui::Button(SET_ICON(Icon::SETTINGS)))
    {
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);
    }

    if (ImGui::Button(SET_ICON(Icon::ON_OFF)))
    {
        state_machine.lastState();
    }

    ImGui::EndMainMenuBar();

    if (show_editor_window)
        renderEditor();

    if (show_metrics_window)
        renderMetrics();

    if (show_animator_window)
        animaton_editor.gui();
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    registry.update(registry.getRegistry(), dtime, animator, pathfinding, window);
    data.camera.update(dtime);
    animaton_editor.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    beginView(target);

    battle_map.draw(*target, sf::RenderStates::Default);
    PathfindingRenderer::render(pathfinding, window);
    registry.draw(registry.getRegistry(), window);
    gizmos.draw();
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
    ImGui::Begin(SET_ICON_TEXT((Icon::MAP), " Editor"), nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    //renderTools();
    renderLayersSection();
    renderTilesSection();

    ImGui::End();
}

void BattleState::renderTools()
{
    //if (ImGui::CollapsingHeader(SET_ICON_TEXT((Icon::TOOL), "Tools")))
    {
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        sf::Color green(40, 159, 49, 255);
        ImVec4 imVecColor(
            green.r / 255.0f,
            green.g / 255.0f,
            green.b / 255.0f,
            green.a / 255.0f);

        auto renderToolButton = [&](Icon icon, const char* id, ToolState tool, GizmoMode mode)
            {
                bool isPressed = tools == tool;
                if (isPressed)
                    ImGui::PushStyleColor(ImGuiCol_Button, imVecColor);

                if (ImGui::Button((ICON::getStr(icon) + std::string(" ##") + id).c_str()))
                {
                    if (tool == ToolState::Brush)
                        is_brash = !is_brash;
                    else
                        is_brash = false;

                    tools       = tool;
                    gizmos.mode = mode;
                }

                if (isPressed)
                    ImGui::PopStyleColor();
            };

        // Select
        renderToolButton(Icon::SELECT, "Select", ToolState::None, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Move
        renderToolButton(Icon::OPEN_WITHIN, "Move", ToolState::Translate, GizmoMode::Translate);
        ImGui::SameLine(0.0f, 1.0f);

        // Scale
        renderToolButton(Icon::UNWRAP, "Scale", ToolState::Scale, GizmoMode::Scale);
        ImGui::SameLine(0.0f, 1.0f);

        // Rotate
        renderToolButton(Icon::UPDATE, "Rotate", ToolState::Rotate, GizmoMode::Rotate);
        ImGui::SameLine(0.0f, 1.0f);

        // Flip
        renderToolButton(Icon::FLIP_HORiZONTAL, "Flip", ToolState::Flip, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Space
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        // Brush
        renderToolButton(Icon::BRUSH, "Brush", ToolState::Brush, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Fill
        renderToolButton(Icon::FILL, "Fill", ToolState::Fill, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        // Space
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();

        // Undo
        renderToolButton(Icon::UNDO, "Undo", ToolState::Undo, GizmoMode::None);
        ImGui::SameLine(0.0f, 1.0f);

        //Redo
        renderToolButton(Icon::REDO, "Redo", ToolState::Redo, GizmoMode::None);

        //if (ImGui::Button((ICON::getStr(Icon::UNDO)).c_str()))
        //    editor.undo();
        //ImGui::SameLine(0.0f, 1.0f);

        //if (ImGui::Button((ICON::getStr(Icon::REDO)).c_str()))
        //    editor.redo();

        // Gizmo
        gizmos.drawImGui();
    }
}

void BattleState::renderLayersSection()
{
    if (ImGui::CollapsingHeader(SET_ICON_TEXT((Icon::STACK_FILES),"Layers")))
    {
        ImGui::Text("Current layer: %d", battle_map.getCurrentLayerID());

        renderTilesetSelector();
        renderLayerControls();
    }
}

void BattleState::renderTilesetSelector()
{
    static std::vector<const char*> items = { "Tileset1.png", "Tileset2.png", "Tileset3.png" };
    static int current_item = 0;
    static bool show_tileset_selector = false;

    if (ImGui::Button(SET_ICON_TEXT((Icon::ADD_FILES),"Add")))
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

    if (ImGui::Button(SET_ICON_TEXT((Icon::REMOVE_FILES)," Remove")))
    {
        battle_map.removeLayer();
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
        const auto& layers = battle_map.getLayers();

        for (size_t i = 0; i < layers.size(); ++i)
        {
            Layer* layer                   = layers[i].get();
            std::string default_layer_name = " Layer " + std::to_string(i);
            std::string display_layer_name = SET_ICON_TEXT((Icon::EMPTY_FILES), (layer->layer_name.empty() ? default_layer_name : layer->layer_name));

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::PushID(static_cast<int>(i));
            ImGui::Checkbox("##Visible", &layer->visible);
            ImGui::PopID();

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Selectable(display_layer_name.c_str(), battle_map.getCurrentLayerID() == i))
            {
                battle_map.setCurrentLayerID(i);
            }

            ImGui::TableSetColumnIndex(2);
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button(SET_ICON(Icon::EDIT)))
            {
                ImGui::OpenPopup("Rename Layer");
            }

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

void BattleState::renderTilesSection()
{
    if (ImGui::CollapsingHeader(SET_ICON_TEXT((Icon::FOUR_QUADS), "Tiles")))
    {
        renderTilesetTable();
    }
}

void BattleState::renderTilesetTable()
{
    static int value     = battle_map.getTileSize();
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


    auto gridPos = battle_map.getMouseGridPosition();

    ImGui::Columns(4, "table_columns##");
    ImGui::Separator();

    ImGui::Text("Coordinates2"); ImGui::SameLine();
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
    ImGui::Button("X##"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%i", gridPos.x);
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    ImGui::Button("Y##"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("%i", gridPos.y);
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(23, 68, 210));
    ImGui::Button("Z##"); ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::Text("1");

    ImGui::Columns(1);

    ImGui::Text("Tile id: %i", battle_map.getLayers().at(0)->getTileId(common::mouse_pos_grid));

    ImGui::Checkbox("Show path", &pathfinding.is_path_visible);
    ImGui::Checkbox("Show connections", &pathfinding.is_connections_visible);
    ImGui::Checkbox("Show nodes", &pathfinding.is_nodes_visible);
    ImGui::Checkbox("Show cost", &pathfinding.is_beginend_visible);

    static char filePath[256] = "map_save.json";
    ImGui::InputText("File Path", filePath, IM_ARRAYSIZE(filePath));
    if (ImGui::Button("Save map"))
    {
        battle_map.saveMap(filePath);
    }

    if (ImGui::Button("Load map"))
    {
        battle_map.loadMap(filePath);
    }

    ImGui::End();
}