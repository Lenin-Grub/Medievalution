#include "stdafx.h"
#include "GameState.hpp"
#include "../StateMachine/StateMachine.hpp"
#include "../MenuState/MenuState.hpp"

GameState::GameState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, world_map()
, is_loaded(false)
, firstNode(nullptr)
, secondNode(nullptr)
{
    state_machine.is_init = true;
}

void GameState::init()
{
    data.camera.setDefaulatView();
    data.camera.reset();
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
                    pathfinding.connect(firstNode, secondNode, 1);
                    firstNode  = nullptr;
                    secondNode = nullptr;
                }
            }
        }
    }
}

void GameState::updateImGui()
{
    renderMenu();
    renderHelp();
    renderNodesTree();
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
    //data.camera.focusOn(position);
}

void GameState::draw(sf::RenderTarget* target)
{
    beginView(target);

    world_map.draw(*target, sf::RenderStates::Default);
    pathfinding.draw(window);

    endView(target);
    ImGui::SFML::Render(window);
}




void GameState::endView(sf::RenderTarget* target)
{
    target->setView(window.getDefaultView());
    target->setView(common::view);
}

void GameState::beginView(sf::RenderTarget*& target)
{
    if (!target)
        target = &window;
    target->setView(common::view);
}



void GameState::renderMenu() 
{
    ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button((Localization::getInstance().get("T_exit")).c_str(), ImVec2(120, 0)))
        state_machine.lastState();

    ImGui::SameLine();

    if (ImGui::Button((ICON::getChar(Icon::SETTINGS))))
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);

    ImGui::End();
}

void GameState::renderHelp() 
{
    std::string str = world_map.getProvinceName(world_map.getColor());
    char* chr = const_cast<char*>(str.c_str());

    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::TextColored(ImVec4(1, 1, 1, 0.5), (char*)"Это карта мира\n" "Используйте WSAD для навигации\n" "Используй колесико мышки для масштабирования\n" "Зажми колесико и перетащи мышь для перемещения по карте\n" "Это и многие другие окна можно передвинуть ЛКМ");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, WindowSettings::getInstance().getFPS());
    ImGui::Separator();

    ImGui::Columns(2, "table_columns");
    ImGui::Separator();
    ImGui::Text("Mouse Window Position:"); 
    ImGui::NextColumn();

    if (ImGui::IsMousePosValid())
        ImGui::Text("(%.1f , %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    else 
        ImGui::Text("Mouse Position: <invalid>");
    ImGui::NextColumn();

    ImGui::Text("Mouse View Position:");
    ImGui::NextColumn();

    ImGui::Text("(%.1f , %.1f)", common::mouse_pos_view.x, common::mouse_pos_view.y);
    ImGui::NextColumn();

    ImGui::Text(Localization::getInstance().get("T_Province_name").c_str()); 
    ImGui::NextColumn();

    ImGui::Text("%s", chr);
    ImGui::NextColumn();

    ImGui::Text(Localization::getInstance().get("T_Province_id").c_str());
    ImGui::NextColumn();

    ImGui::Text("%d", world_map.getProvinceID(world_map.getColor()));
    ImGui::NextColumn();

    ImGui::Separator();
    ImGui::Columns(1);

    ImGui::SliderFloat(Localization::getInstance().get("T_transparency").c_str(), &world_map.transparency, 0.0f, 1.0f);
    ImGui::End();
}

void GameState::renderNodesTree()
{
    ImGui::Begin("Nodes Tree");
    ImGui::InputText((ICON::getStr(Icon::SEARCH) + " Search").c_str(), search_buffer, IM_ARRAYSIZE(search_buffer));
    ImGui::SameLine();

    if (ImGui::Button("Reset"))
        memset(search_buffer, 0, sizeof(search_buffer));

    std::vector<const Node*> filtered_nodes;
    std::string search_query = search_buffer;

    if (ImGui::CollapsingHeader((std::string(ICON::getStr(Icon::DOT_MENU)) + " Provinces").c_str()))
    {
        for (const auto& pair : pathfinding.nodes)
        {
            const Node& node = pair.second;

            if (node_names.find(&node) == node_names.end())
            {
                sf::Color   node_color    = world_map.getColor(pair.second.position);
                std::string province_name = world_map.getProvinceName(world_map.getColor(pair.second.position), pair.second.position);
                node_names.at(& node)     = province_name;
                nodeIDs.at(& node)        = world_map.getProvinceID(world_map.getColor(pair.second.position), pair.second.position);
            }

            std::string_view nodeName = node_names[&node];
            int nodeID                = nodeIDs[&node];

            if (nodeName.find(search_query) != std::string_view::npos || std::to_string(nodeID).find(search_query) != std::string::npos)
                filtered_nodes.push_back(&node);
        }

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(filtered_nodes.size()));

        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
            {
                const Node* node = filtered_nodes[i];

                bool open = ImGui::TreeNode((void*)(intptr_t)node, "Node: %s \t (ID: %d)", node_names[node].c_str(), nodeIDs[node]);

                ImGui::SameLine();
                ImGui::PushID(node);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));

                if (ImGui::Button((std::string(ICON::getStr(Icon::LOCATION))).c_str()))
                {
                    position = node->position;
                    data.camera.setFocusOn(position);
                    data.camera.enableFocus(true);
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();

                if (open)
                {
                    for (const Node* neighbor : node->neighbors)
                    {
                        if (node_names.find(neighbor) == node_names.end())
                        {
                            sf::Color neighbor_color           = world_map.getColor(neighbor->position);
                            std::string neighbor_province_name = world_map.getProvinceName(world_map.getColor(neighbor->position), neighbor->position);
                            node_names[neighbor]               = neighbor_province_name;
                            nodeIDs[neighbor]                  = world_map.getProvinceID(world_map.getColor(neighbor->position), neighbor->position);
                        }

                        ImGui::BulletText("Neighbor: %s \t (ID: %d)", node_names[neighbor].c_str(), nodeIDs[neighbor]);
                        ImGui::SameLine();

                        ImGui::PushID(neighbor);
                        if (ImGui::Button("X"))
                            pathfinding.disconnect(const_cast<Node*>(node), const_cast<Node*>(neighbor));
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
            }
        }

        clipper.End();
    }

    ImGui::End();
}
