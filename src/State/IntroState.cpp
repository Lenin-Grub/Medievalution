#include "../stdafx.h"
#include "IntroState.hpp"
#include "MenuState.hpp"
#include "StateMachine.hpp"

IntroState::IntroState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, alpha { 255, 255, 255, 0 }
{
    LOG_INFO("State Intro\t Init");
    setBackground();
}

void IntroState::init()
{
}

void IntroState::onDeactivate()
{
    LOG_INFO("State Intro\t Deactivate");
}

void IntroState::onActivate()
{
    LOG_INFO("State Intro\t Activate");
}

void IntroState::updateEvents()
{
    if (Input::isKeyPressed(sf::Keyboard::Key::Space))
    {
        next_state = StateMachine::build<MenuState>(data, state_machine, window, true);
    }
}

void IntroState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration 
                            |   ImGuiWindowFlags_AlwaysAutoResize 
                            |   ImGuiWindowFlags_NoFocusOnAppearing 
                            |   ImGuiWindowFlags_NoNav 
                            |   ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Press \"SPACEBAR\" to continue");
    ImGui::End();
}

void IntroState::update(const float& dtime)
{
    if (alpha.a != 255)
    {
        alpha.a++;
    }
    else
    {
        next_state = StateMachine::build<MenuState>(data, state_machine, window, true);
    }
    text.setFillColor(alpha);
}

void IntroState::draw(sf::RenderTarget* target)
{
    window.clear();

    window.draw(shape);
    window.draw(text);

    ImGui::SFML::Render(window);
    
    window.display();
}

void IntroState::setBackground()
{
    if (!background.loadFromFile("resources/Backgrounds/ruszastavka.png"))
    {
        LOG_ERROR("File \"background\" not found");
    }
    if (!font.loadFromFile("resources/Fonts/Blackmoor.ttf"))
    {
        LOG_ERROR("File \"font\" not found");
    }

    shape.setTexture(&background);
    shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

    text.setFont(font);
    text.setString("MedievalutioN");
    text.setCharacterSize(core::math.convertToPercentage(window.getSize().x, 10));
    text.setFillColor(alpha);

    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 0.4f);
    text.setPosition(core::math.setCentre(window.getSize().x, window.getSize().y));
}