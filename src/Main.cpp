#include "stdafx.h"
#include "Game/Game.h"

// Off console in release version.
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    Game game;
    game.run();
    return 0;
}

//
//#include <SFML/Graphics.hpp>
//#include <imgui.h>
//#include <imgui-SFML.h>
//
//int main() {
//    // Создаем окно SFML
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + ImGui Docking Example");
//    ImGui::SFML::Init(window);
//
//    // Основной цикл приложения
//    sf::Clock deltaClock;
//    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Включаем докирование
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            ImGui::SFML::ProcessEvent(event);
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        // Начинаем новый кадр ImGui
//        ImGui::SFML::Update(window, deltaClock.restart());
//
//        // Создаем интерфейс ImGui
//        ImGui::Begin("Docking Example");
//
//        // Создаем докируемую область
//        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//        ImGui::DockSpace(dockspace_id);
//
//        // Пример создания нескольких окон
//        if (ImGui::Begin("Window 1")) {
//            ImGui::Text("This is window 1");
//            ImGui::End();
//        }
//
//        if (ImGui::Begin("Window 2")) {
//            ImGui::Text("This is window 2");
//            ImGui::End();
//        }
//
//        ImGui::End();
//
//        // Отрисовка ImGui
//        window.clear();
//        ImGui::SFML::Render(window);
//        window.display();
//    }
//
//    // Освобождаем ресурсы
//    ImGui::SFML::Shutdown();
//    return 0;
//}