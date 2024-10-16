#include "../stdafx.h"
#include "StateMachine.hpp"
#include "LoadingState.hpp"

LoadingState::LoadingState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
, alpha { 255, 255, 255, 255 }
{
    state_machine.is_init = true;
}

void LoadingState::init()
{
    setBackground();
    LOG_INFO("State Loading\t Init");
}

void LoadingState::onDeactivate()
{
    LOG_INFO("State Loading\t Deactivate");
}

void LoadingState::onActivate()
{
    LOG_INFO("State Loading\t Activate"); 
}

void LoadingState::updateEvents()
{
}

void LoadingState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T2", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    ImGui::End();
}

void LoadingState::update(const float& dtime)
{
}

void LoadingState::draw(sf::RenderTarget* target)
{
    auto load_state_future = std::async(std::launch::async, [&]()
        {
            auto load_state = StateMachine::build<GameState>(data, state_machine, window, true);
            return load_state;
        });

    window.draw(shape);
    window.draw(text);
    ImGui::SFML::Render(window);
    window.display();

    auto load_state = load_state_future.get();
    next_state = std::move(load_state);
}
void LoadingState::setBackground()
{
    background = ResourceLoader::instance().getTexture("background_1.png");
    font = ResourceLoader::instance().getFont("Blackmoor.ttf");

    shape.setTexture(&background);
    shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

    text.setFont(font);
    text.setString("Loading");
    text.setCharacterSize(common::math.convertToPercentage(window.getSize().x, 10));
    text.setFillColor(alpha);

    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 0.4f);
    text.setPosition(common::math.setCentre(window.getSize().x, window.getSize().y));
}
