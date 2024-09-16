#include "../stdafx.h"
#include "StateMachine.hpp"
#include "BattleState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, camera()
, battleground()
{
	LOG_INFO("State Battle\t Init");
	pathfinding.initNodes(100, 100);
	    const int level[] =
    {
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 16, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 16, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
        6,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 46, 1, 1, 1, 1,
        1,  1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16, 16, 16, 16,
        1,  1, 1, 1, 46, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 16, 16, 1, 1, 1, 1, 1, 1, 46, 1, 1,
        1,  1, 1, 1, 16, 1, 46, 46, 1, 1, 1, 1, 1, 1, 46, 1,
        46, 1, 1, 1, 16, 1, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1,
        1,  1, 1, 1, 16, 46, 46, 46, 1, 1, 1, 1, 1, 1, 1, 1,
    };
	battleground.load("resources/Map/tileset.png", sf::Vector2u(64, 64), level, 16, 16);

    try
    {
        SpriteSheet sprite_sheet("Map/tileset.png", 32);

        sprite_sheet.import(m_board);

        m_size_error_msg = false;
        m_file_error_msg = false;
        m_sprite_sheet = sprite_sheet;
        m_imported_sheet = true;
    }
    catch (const std::exception& ex)
    {
        // If an integer is not entered display tile size error
        if (dynamic_cast<const std::invalid_argument*>(&ex))
        {
            m_size_error_msg = true;
        }
    }
    m_board.initBoard();
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

	if (Input::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		id++;
	}
	if (Input::isKeyPressed(sf::Keyboard::Key::Num2))
	{
		id--;
	}


	if (Input::isMouseReleased(sf::Mouse::Left))
	{
		sf::Vector2u tilePos = sf::Vector2u(core::mouse_pos_view.x / 64, core::mouse_pos_view.y / 64);
		battleground.update(tilePos.x, tilePos.y, id);
		
	}

    if (Input::isMouseReleased(sf::Mouse::Left) && !ImGui::GetIO().WantCaptureMouse)
    {
        if (m_imported_sheet)
        {
            sf::Vector2f position = core::mouse_pos_view;
            m_sprite_sheet.add_tile_id(m_selected_tile_id, position.x, position.y);
        }
    }

	//pathfinding.handleInput();
	camera.scroll();
	camera.zoom();
}

void BattleState::updateImGui()
{
	ImGui::Begin("GameMenu###", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button((char*)ICON_LOG_IN "Выход###toMainMenu", ImVec2(120, 0)))
	{
		state_machine.lastState();
	}
	ImGui::End();

	//ImGui::Begin("Editor", nullptr);
	//std::string message = "World editor";
	//ImGui::Text(message.data(), message.data() + message.size());
	//ImGui::Text("Tile id: %d", id);
	//ImGui::Image(battleground.m_tileset);
	//ImGui::End();

    ImGui::Begin("Editor", nullptr);
    
    if (m_imported_sheet)
    {
        // Create a child window with scrolling
        ImGui::BeginChild("Tileset", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        static int value = 32;   // Initial scale value
        const int minValue = 8;  // Minimum scale value
        const int maxValue = 64; // Maximum scale value

        ImGui::SliderInt("Scale", &value, minValue, maxValue);

        sf::Texture& tileset_Texture = m_sprite_sheet.get_tileset_texture();
        int tileset_cols = m_sprite_sheet.get_sheet_width();
        int tileset_rows = m_sprite_sheet.get_sheet_height();

        ImTextureID tilesetTextureId = (ImTextureID)(intptr_t)tileset_Texture.getNativeHandle(); // Cast the texture ID to ImTextureID

        ImVec2 scale_factor = ImVec2(value, value);

        ImGui::BeginTable("TilesetTable", tileset_cols, ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);
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
        ImGui::EndChild();
    }
    ImGui::End();
}

void BattleState::update(const float& dtime)
{
	updateMousePositions();
	//pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
	camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
	window.clear();
	if (!target)
		target = &window;
	target->setView(core::view);

	//pathfinding.draw(window);
    //target->draw(battleground);
    target->draw(m_board);

    if (m_imported_sheet)
    {
        m_sprite_sheet.merge_tiles();
        target->draw(m_sprite_sheet);
    }

	target->setView(window.getDefaultView());
	target->setView(core::view);

	ImGui::SFML::Render(window);

	window.display();
}
