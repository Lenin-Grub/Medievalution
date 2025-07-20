#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui-SFML.h>

#include <entt/entt.hpp>
#include <json.hpp>

#include "../Engine/Camera/Camera.h"
#include <Common/Common.h>
#include <Common/IconText.h>
#include <Common/Log.h>

#include <Resource/ResourceManager.hpp>

#include <Animation/AnimationEditor/AnimationEditor.hpp>
#include <Animation/Animator/Animator.hpp>

#include "../../Game/World/BattleMap/BattleMap.hpp"

// Off console in release version.
#ifdef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// to do вынести в engine?
void updateMousePositions(sf::View* view, sf::RenderWindow& window)
{
    common::mouse_pos_screen = sf::Mouse::getPosition();
    common::mouse_pos_window = sf::Mouse::getPosition(window);
    common::mouse_pos_view   = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

bool initIcon(sf::RenderWindow& window)
{
    try
    {
        sf::Image icon = ResourceLoader::instance().getImage("map.png");
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        return true;
    }
    catch (const std::exception& e)
    {
        LOG_CRITICAL("Failed init icon");
        return false;
    }
}

bool initFonts()
{
    try
    {
        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;
        config.GlyphMinAdvanceX = 6.0f;
        config.OversampleH = 3;
        config.OversampleV = 3;

        static const ImWchar icon_ranges[] = { static_cast<ImWchar> (Icon::MIN), static_cast<ImWchar>(Icon::MAX), 0 };

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font1 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ImFont* font2 = io.Fonts->AddFontFromFileTTF("resources/Fonts/MedievalutionIcons.ttf", 20.f, &config, icon_ranges);
        if (io.Fonts->Build())
        {
            ImGui::SFML::UpdateFontTexture();
            io.FontDefault = font1;
        }
        return true;
    }
    catch (const std::exception& e)
    {
        LOG_CRITICAL("Failed init fonts");
        return false;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Medievaltuion editor");

    initIcon(window);
    ImGui::SFML::Init(window);
    initFonts();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    window.setFramerateLimit(60);

    auto window_size = (sf::Vector2f)window.getSize();
    Camera camera(window_size, common::view);
    camera.setWindowSize(window_size);


    sf::RenderTexture renderTexture;
    renderTexture.create(800, 600);

    AnimationEditor animation_editor(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        auto delta_time = common::clock.restart().asSeconds();

        while (window.pollEvent(common::sfml_event))
        {
            if (common::sfml_event.type == sf::Event::Closed)
                window.close();

            if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }
            updateMousePositions(&common::view, window);

            ImGui::SFML::ProcessEvent(window, common::sfml_event);
            camera.zoom();
            camera.scroll();
        }

        camera.update(delta_time);

        ImGui::SFML::Update(window, common::clock.restart());

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Window", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoBackground);

        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id);

        ImGui::End();

        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::TOOL), "Map Editor")))
            {
            }

            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::INSTAGRAM), "Animation Editor")))
            {
            }

            if (ImGui::MenuItem(SET_ICON_TEXT((Icon::METRIC_UP), "Metric")))
            {
            }

            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80);

        if (ImGui::Button(SET_ICON(Icon::SETTINGS)))
        {
        }

        if (ImGui::Button(SET_ICON(Icon::ON_OFF)))
        {
            window.close();
        }
        ImGui::EndMainMenuBar();

        ImGui::Begin("Game Viewport");

        ImVec2 avail = ImGui::GetContentRegionAvail();

        if (avail.x > 0 && avail.y > 0) {
            if (renderTexture.getSize().x != static_cast<unsigned int>(avail.x) ||
                renderTexture.getSize().y != static_cast<unsigned int>(avail.y)) {
                renderTexture.create(static_cast<unsigned int>(avail.x), static_cast<unsigned int>(avail.y));
            }

            camera.update(delta_time);
            renderTexture.setView(common::view);
            renderTexture.clear(sf::Color(30, 48, 74));
            //battle_map.draw(window, sf::RenderStates::Default);
            shape.setPosition(100, 100);
            renderTexture.draw(shape);
            renderTexture.display();
        }

        ImTextureID texId = static_cast<ImTextureID>(static_cast<uintptr_t>(renderTexture.getTexture().getNativeHandle()));
        ImGui::Image(texId, avail);

        ImGui::End();

        animation_editor.update(delta_time);
        animation_editor.gui();

        ImGui::Begin("Console");
        ImGui::End();

        ImGui::Begin("Scene");
        ImGui::End();


        window.setView(common::view);

        window.setView(common::view);

        ImGui::SFML::Render(window);
        window.display();

    }

    ImGui::SFML::Shutdown();

    return 0;
}
