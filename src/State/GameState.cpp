#include "../stdafx.h"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"

GameState::GameState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, camera()
, world_map()
, is_loaded(false)
{
    LOG_INFO("State Game\t Init");
    world_map.init();
}

void GameState::init()
{
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
        camera.zoom();
        camera.scroll();

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

    if (ImGui::Button(Localization::getInstance().getStringByKey("T_exit").c_str(), ImVec2(120, 0)))
    {
        state_machine.lastState();
    }
    std::string str1 = WindowSettings::getInstance().localization.at("T_settings");

    if (ImGui::Button(((char*)ICON_SETTINGS + str1).c_str(), ImVec2(120, 0)))
    {
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);
    }
    ImGui::End();

    std::string str = world_map.getProvinceName();
    char* chr = const_cast<char*>(str.c_str());

    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);


    ImGui::TextColored(ImVec4(1, 1, 1, 0.5), (char*)"Это карта мира\n" "Используйте WSAD для навигации\n" "Используй колесико мышки для масштабирования\n" "Зажми колесико и перетащи мышь для перемещения по карте\n" "Это и многие другие окна можно передвинуть ЛКМ");
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    if (ImGui::IsMousePosValid())
        ImGui::Text("Mouse Window Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    else
        ImGui::Text("Mouse Position: <invalid>");

    ImGui::Text("Mouse View Position: (%.1f,%.1f)", core::mouse_pos_view.x, core::mouse_pos_view.y);
    ImGui::Text(Localization::getInstance().getStringByKey("T_Province_name").c_str(), chr);
    ImGui::Text(Localization::getInstance().getStringByKey("T_Province_id").c_str(), world_map.getProvinceID());

    ImGui::Separator();
    ImGui::SliderFloat(Localization::getInstance().getStringByKey("T_transparency").c_str(), &world_map.transparency, 0.0f, 1.0f);

    //{
    //	// string � imgui
    //	std::string message = "Hello world";
    //	ImGui::Text(message.data(), message.data() + message.size());
    //}

    ImGui::End();
}

void GameState::update(const float& dtime)
{
    sf::Color color = world_map.getColor();

    world_map.select_color = color;
    world_map.shader.setParameter("select_color", world_map.select_color);
    world_map.shader.setParameter("transparency", world_map.transparency);

    updateMousePositions();
    camera.update(dtime);
}

void GameState::draw(sf::RenderTarget* target)
{
    window.clear();
    if (!target)
        target = &window;
    target->setView(core::view);

    world_map.draw(*target, sf::RenderStates::Default);

    target->setView(window.getDefaultView());
    target->setView(core::view);

    ImGui::SFML::Render(window);

    window.display();
}

bool GameState::isLoad()
{
    if (world_map.initProvinceData())
    {
        is_loaded = true;
    }
    return is_loaded;
}