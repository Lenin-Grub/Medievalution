#include "../stdafx.h"
#include "StateMachine.hpp"
#include "MenuState.hpp"

MenuState::MenuState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
{
        state_machine.is_init = true;
}

void MenuState::init()
{
    setBackground();
    LOG_INFO("State Menu\t Init");
}

void MenuState::onDeactivate()
{
    LOG_INFO("State Menu\t Deactivate");
}

void MenuState::onActivate()
{
    LOG_INFO("State Menu\t Activate");
}

void MenuState::updateEvents()
{
    if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        state_machine.quit();
    }
}

void MenuState::updateImGui()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar
                                     | ImGuiWindowFlags_NoCollapse
                                     | ImGuiWindowFlags_AlwaysAutoResize
                                     | ImGuiWindowFlags_NoResize
                                     | ImGuiWindowFlags_NoMove);

    if (ImGui::Button(Localization::getInstance().get("T_new_game").c_str(), ImVec2(120, 0)))
    {
        next_state = StateMachine::build<LoadingState>(data, state_machine, window, false);
    }
    
    if (ImGui::Button(Localization::getInstance().get("T_battle_scene").c_str(), ImVec2(120, 0)))
    {
        next_state = StateMachine::build<BattleState>(data, state_machine, window, false);
    }

    if (ImGui::Button(Localization::getInstance().get("T_settings").c_str(), ImVec2(120, 0)))
    {
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);
    }

    if (ImGui::Button(Localization::getInstance().get("T_exit").c_str(), ImVec2(120, 0)))
    {
        state_machine.quit();
    }
    ImGui::End();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void MenuState::update(const float& dtime)
{

}

void MenuState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(window.getDefaultView());

    window.draw(shape);

    ImGui::SFML::Render(window);
}

void MenuState::setBackground()
{
    if (!background.loadFromFile("resources/Backgrounds/background_main.jpg"))
    {
        LOG_ERROR("File \"background_main.jpg\" not found");
    }

    shape.setTexture(&background);
    shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
}