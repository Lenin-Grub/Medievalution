#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui-SFML.h>

#include <entt/entt.hpp>
#include <json.hpp>

// Off console in release version.
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Medievaltuion editor");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock delta_clock;
    sf::Event event;

    while (window.isOpen())
    {
        auto delta_time = delta_clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            ImGui::SFML::ProcessEvent(window, event);
        }

        ImGui::SFML::Update(window, delta_clock.restart());


        ImGui::Begin("Hello, world!");

        if (ImGui::Button("Look at this pretty button"))
            spdlog::info("Event work!");

        ImGui::End();

        window.clear(sf::Color(30, 48, 74));

        window.draw(shape);
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
