#include "../stdafx.h"
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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f provinceCenter = world_map.findProvinceCenter(world_map.getColor());
            world_map.shape.setPosition(provinceCenter);
        }
    }
}

void GameState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::ShowDemoWindow();
    ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button((Localization::getInstance().get(ICON_LOG_OUT, "T_exit")).c_str(), ImVec2(120, 0)))
    {
        state_machine.lastState();
    }

    if (ImGui::Button(Localization::getInstance().get(ICON_SETTINGS, "T_settings").c_str(), ImVec2(120, 0)))
    {
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);
    }
    ImGui::End();

    std::string str = world_map.getProvinceName(world_map.getColor());
    char* chr = const_cast<char*>(str.c_str());

    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::TextColored(ImVec4(1, 1, 1, 0.5), (char*)"Это карта мира\n" "Используйте WSAD для навигации\n" "Используй колесико мышки для масштабирования\n" "Зажми колесико и перетащи мышь для перемещения по карте\n" "Это и многие другие окна можно передвинуть ЛКМ");
    
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
    //{
    //	// string � imgui
    //	std::string message = "Hello world";
    //	ImGui::Text(message.data(), message.data() + message.size());
    //}
}

void GameState::update(const float& dtime)
{
    sf::Color color = world_map.getColor();

    world_map.select_color = color;
    world_map.shader.setParameter("select_color", world_map.select_color);
    world_map.shader.setParameter("transparency", world_map.transparency);

    updateMousePositions();
    data.camera.update(dtime);
}

void GameState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(common::view);

    world_map.draw(*target, sf::RenderStates::Default);

    target->setView(window.getDefaultView());
    target->setView(common::view);

    ImGui::SFML::Render(window);
}