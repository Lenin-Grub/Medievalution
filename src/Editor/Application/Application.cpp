#include "Application.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

#include <Resource/ResourceManager.hpp>
#include <Common/IconText.h>
#include <Common/Log.h>
#include <Input/Input.h>

EditorApplication::EditorApplication()
    : window(sf::VideoMode(1920, 1200), "Medievalution Editor")
    , clear_color(43, 43, 49)
    , displays(window, battle_map)
    , camera(static_cast<sf::Vector2f>(window.getSize()), common::view)
{
    setupWindow();
    setupImGui();
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::run()
{
    dtime = clock.restart().asSeconds();

    if (!init())
        return;

    while (window.isOpen())
    {
        updateEvents();
        update();
        draw();
    }
    ImGui::SFML::Shutdown();
}

bool EditorApplication::init()
{
    if (!battle_map.init())
    {
        LOG_WARN("Battle map not inited");
        return false;
    }

    return true;
}

void EditorApplication::updateMousePositions(sf::View* view, sf::RenderWindow& window)
{
    common::mouse_pos_screen = sf::Mouse::getPosition();
    common::mouse_pos_window = sf::Mouse::getPosition(window);
    common::mouse_pos_view = window.mapPixelToCoords(sf::Mouse::getPosition(window), *view);
}

bool EditorApplication::initIcon(sf::RenderWindow& window)
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

bool EditorApplication::initFonts()
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
        ImFont* font1 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf", 25.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ImFont* font2 = io.Fonts->AddFontFromFileTTF("resources/Fonts/MedievalutionIcons.ttf", 25.f, &config, icon_ranges);
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

void EditorApplication::setupImGui()
{
    ImGui::SFML::Init(window);
    initFonts();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}

void EditorApplication::setupWindow()
{
    window.setFramerateLimit(60);
    sf::Vector2f window_size = static_cast<sf::Vector2f>(window.getSize());

    initIcon(window);
}

void EditorApplication::updateEvents()
{
    while (window.pollEvent(common::sfml_event))
    {
        ImGui::SFML::ProcessEvent(common::sfml_event);

        if (Input::isKeyPressed(sf::Keyboard::Key::Escape) || common::sfml_event.type == sf::Event::Closed)
            window.close();

        battle_map.updatePreview(common::mouse_pos_view);

        //if (true)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                battle_map.addTile(battle_map.getTileId(), common::mouse_pos_view);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                battle_map.removeTile(common::mouse_pos_view);
        }

        if (!ImGui::GetIO().WantCaptureMouse || !ImGui::GetIO().WantCaptureKeyboard)
        {
            camera.zoom(common::sfml_event);
            camera.scroll(common::sfml_event, sf::Mouse::getPosition(window));
        }

        ImGui::SFML::ProcessEvent(window, common::sfml_event);
    }
}

void EditorApplication::update()
{
    ImGui::SFML::Update(window, clock.restart());
    updateMousePositions(&common::view, window);
    camera.update(dtime);
    displays.update(dtime);
}

void EditorApplication::draw(sf::RenderTarget* target)
{
    window.clear(clear_color);

    displays.draw();

    ImGui::SFML::Render(window);
    window.display();
}
