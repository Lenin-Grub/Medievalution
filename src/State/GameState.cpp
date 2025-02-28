#include "stdafx.h"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"

GameState::GameState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, world_map()
, is_loaded(false)
{
    state_machine.is_init = true;
}

void GameState::init()
{
    data.camera.setDefaulatView();
    world_map.init();
    LOG_INFO("State Game\t Init");
    texture = ResourceLoader::instance().getTexture("Spearman.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,64,64));

    auto provinceCenters = world_map.provinces;

    for (const auto& entry : provinceCenters)
    {
        pathfinding.addNode(entry.second.centre);
    }
}

void GameState::onDeactivate()
{
    LOG_INFO("State Game\t Deactivate");

}

void GameState::onActivate()
{
    LOG_INFO("State Game\t Activate");
}

void GameState::updateEvents()
{
    if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        state_machine.lastState();
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        data.camera.zoom();
        data.camera.scroll();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) 
        {
            sf::Vector2f provinceCenter = world_map.findProvinceCenter(world_map.getColor());
            world_map.shape.setPosition(provinceCenter);
            pathfinding.start_node = pathfinding.getNode(provinceCenter);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) 
        {
            sf::Vector2f provinceCenter = world_map.findProvinceCenter(world_map.getColor());
            world_map.shape.setPosition(provinceCenter);
            pathfinding.end_node = pathfinding.getNode(provinceCenter);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Color selectedColor = world_map.getColor();
            if (selectedColor != world_map.selected_province_color)
            {
                world_map.selected_province_color = selectedColor;
                world_map.is_selected = true;
            }
        }

        pathfinding.handleInput();

        // Handle input
       if( sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
            sf::Vector2f provinceCenter = world_map.findProvinceCenter(world_map.getColor());
            world_map.shape.setPosition(provinceCenter);

            if (firstNode == nullptr) 
                firstNode = pathfinding.getNode(provinceCenter);
            else 
            {
                secondNode = pathfinding.getNode(provinceCenter);
                if (firstNode && secondNode && firstNode != secondNode)
                {
                    pathfinding.connect(firstNode, secondNode);
                    firstNode  = nullptr;
                    secondNode = nullptr;
                }
            }
        }
    }
}

void GameState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();

//    ImGui::ShowDemoWindow();

#pragma region Menu
    ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button((Localization::getInstance().get("T_exit")).c_str(), ImVec2(120, 0)))
        state_machine.lastState();
    ImGui::SameLine();
    if (ImGui::Button((ICON::toUtf8(ICON_SETTINGS)).c_str()))
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);
    ImGui::End();
#pragma endregion

#pragma region Help

    std::string str = world_map.getProvinceName(world_map.getColor());
    char* chr = const_cast<char*>(str.c_str());

    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::TextColored(ImVec4(1, 1, 1, 0.5), (char*)"Это карта мира\n" "Используйте WSAD для навигации\n" "Используй колесико мышки для масштабирования\n" "Зажми колесико и перетащи мышь для перемещения по карте\n" "Это и многие другие окна можно передвинуть ЛКМ");
    
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    ImGui::Separator();

    //--------
    ImGui::Columns(2, "table_columns");
    ImGui::Separator();
    ImGui::Text("Mouse Window Position:");                                              ImGui::NextColumn();
    if (ImGui::IsMousePosValid())
        ImGui::Text("(%.1f , %.1f)", io.MousePos.x, io.MousePos.y); 
    else ImGui::Text("Mouse Position: <invalid>");
                                                                                        ImGui::NextColumn();
    ImGui::Text("Mouse View Position:");                                                ImGui::NextColumn();
    ImGui::Text("(%.1f , %.1f)", common::mouse_pos_view.x, common::mouse_pos_view.y);   ImGui::NextColumn();
    ImGui::Text(Localization::getInstance().get("T_Province_name").c_str()); ImGui::NextColumn();
    ImGui::Text("%s", chr);                                                             ImGui::NextColumn();
    ImGui::Text(Localization::getInstance().get("T_Province_id").c_str());   ImGui::NextColumn();
    ImGui::Text("%d",world_map.getProvinceID(world_map.getColor()));                    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Columns(1);
    ImGui::SliderFloat(Localization::getInstance().get("T_transparency").c_str(), &world_map.transparency, 0.0f, 1.0f);
    ImGui::End();

#pragma endregion
        
#pragma region Nodes_and_neighbors_tree
    // дорогостоящая операция

    ImGui::Begin("Nodes Tree");

    static std::unordered_map<const Node*, std::string> nodeNames;              // Variable to store node names
    static std::unordered_map<const Node*, int>         nodeIDs;                // Variable to store node IDs
    static char                                         searchBuffer[128] = ""; // Buffer for search input

    // Search input field
    ImGui::InputText((ICON::toUtf8(ICON_SEARCH) + " Search").c_str(), searchBuffer, IM_ARRAYSIZE(searchBuffer));

    // Reset button
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
        memset(searchBuffer, 0, sizeof(searchBuffer)); // Clear the search buffer

    // Filter nodes based on the search query
    std::vector<const Node*> filteredNodes;
    std::string searchQuery = searchBuffer;

    
    for (const auto& pair : pathfinding.nodes)
    {
        const Node& node = pair.second;

        if (nodeNames.find(&node) == nodeNames.end())
        {
            sf::Color nodeColor = world_map.getColor();
            std::string provinceName = world_map.getProvinceName(nodeColor);
            nodeNames[&node] = provinceName;
            nodeIDs[&node] = world_map.getProvinceID(nodeColor);
        }

        std::string_view nodeName = nodeNames[&node];
        int nodeID = nodeIDs[&node];

        // Check if the node name or ID matches the search query
        if (nodeName.find(searchQuery) != std::string_view::npos || std::to_string(nodeID).find(searchQuery) != std::string::npos)
            filteredNodes.push_back(&node);
    }

    // Display filtered nodes
    for (const Node* node : filteredNodes)
    {
        if (ImGui::TreeNode((void*)(intptr_t)node, "Node: %s \t (ID: %d)", nodeNames[node].c_str(), nodeIDs[node]))
        {
            for (const Node* neighbor : node->neighbors)
            {
                if (nodeNames.find(neighbor) == nodeNames.end())
                {
                    sf::Color neighborColor          = world_map.getColor();
                    std::string neighborProvinceName = world_map.getProvinceName(neighborColor);
                    nodeNames[neighbor]              = neighborProvinceName;
                    nodeIDs[neighbor]                = world_map.getProvinceID(neighborColor);
                }

                ImGui::BulletText("Neighbor: %s \t (ID: %d)", nodeNames[neighbor].c_str(), nodeIDs[neighbor]);
                ImGui::SameLine();

                ImGui::PushID(neighbor);
                if (ImGui::Button("X"))
                    pathfinding.disconnect(const_cast<Node*>(node), const_cast<Node*>(neighbor));
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();

#pragma endregion

}

void GameState::update(const float& dtime)
{
    sf::Color color = world_map.getColor();

    world_map.hover_color  = color;
    world_map.select_color = world_map.selected_province_color;
    world_map.shader.setParameter("select_color", world_map.select_color);
    world_map.shader.setParameter("transparency", world_map.transparency);
    world_map.shader.setParameter("is_selected", world_map.is_selected);
    world_map.shader.setParameter("hover_color", world_map.hover_color);

    updateMousePositions();
    data.camera.update(dtime);
    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);

    pathfinding.move(dtime);

    // Обновляем позицию квадрата
    if (pathfinding.current_node != nullptr && pathfinding.start_node != nullptr && pathfinding.end_node != nullptr)
    {
        sprite.setPosition(pathfinding.current_node->position.x, pathfinding.current_node->position.y);
    }
}

void GameState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(common::view);

    world_map.draw(*target, sf::RenderStates::Default);
    pathfinding.draw(window);

    target->draw(sprite);

    target->setView(window.getDefaultView());
    target->setView(common::view);

    ImGui::SFML::Render(window);
}