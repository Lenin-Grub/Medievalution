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
#include <imgui_internal.h>

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


class EditorApplication
{
private:
    sf::RenderWindow window;
    sf::Clock deltaClock;

    // Переменные для виджетов
    bool showDemoWindow = false;
    bool showAnotherWindow = false;
    bool showAnimationWindow = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float floatValue = 0.0f;
    int intValue = 0;
    bool boolValue = true;
    char stringBuffer[128] = "Hello, World!";
    int currentItem = 0;
    const char* items[3] = { "Item 1", "Item 2", "Item 3" };
    AnimationEditor animation_editor;
    Camera camera;
    sf::Vector2f window_size;
    sf::RenderTexture renderTexture;
public:
    EditorApplication() : window(sf::VideoMode(1920, 1200), "Medievalution Editor"), animation_editor(window), window_size((sf::Vector2f)window.getSize()), camera(window_size, common::view)
    {
        window.setFramerateLimit(60);
            initIcon(window);
            ImGui::SFML::Init(window);
            initFonts();

        // Настраиваем ImGui
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        auto window_size = (sf::Vector2f)window.getSize();

        camera.setWindowSize(window_size);
        renderTexture.create(window.getSize().x, window.getSize().y);

    }

    ~EditorApplication()
    {
        ImGui::SFML::Shutdown();
    }

    void Run()
    {
        while (window.isOpen())
        {
            HandleEvents();
            Update();
            Render();
        }
    }

private:
    void HandleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    void Update()
    {
        ImGui::SFML::Update(window, deltaClock.restart());
        animation_editor.update(deltaClock.restart().asSeconds());

        if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window.close();
        }
        updateMousePositions(&common::view, window);

        ImGui::SFML::ProcessEvent(window, common::sfml_event);
        camera.zoom();
        camera.scroll();
    }

    void Render()
    {
        SetupDockSpace();
        CreateWindows();

        window.clear(sf::Color(255, 255, 255, 255));
        ImGui::SFML::Render(window);
        window.display();
    }

    void SetupDockSpace()
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

        if (opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            ImGui::PopStyleVar();

        CreateMenuBar();
        InitializeDockingLayout();

        ImGui::End();
    }

    void CreateMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N")) {}
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4")) { window.close(); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Demo Window", NULL, &showDemoWindow);
                ImGui::MenuItem("Another Window", NULL, &showAnotherWindow);
               if(ImGui::MenuItem("Animation Window", NULL, showAnimationWindow)){ showAnimationWindow = true; }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void InitializeDockingLayout()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

            static auto first_time = true;
            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

                auto center = dockspace_id;
                auto left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.20f, nullptr, &center);
                auto right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.50f, nullptr, &center);
                auto bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.25f, nullptr, &center);
                auto right_bottom = ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.40f, nullptr, &right);

                ImGui::DockBuilderDockWindow("Scene Hierarchy", left);
                ImGui::DockBuilderDockWindow("Object Details", right);
                ImGui::DockBuilderDockWindow("Tileset", right);
                ImGui::DockBuilderDockWindow("Tile Details", right);
                ImGui::DockBuilderDockWindow("Tile Layers", right_bottom);
                ImGui::DockBuilderDockWindow("Scene", center);
                ImGui::DockBuilderDockWindow("Script List", center);
                ImGui::DockBuilderDockWindow("Package Game", center);
                ImGui::DockBuilderDockWindow("Tilemap Editor", center);
                ImGui::DockBuilderDockWindow("Assets", bottom);
                ImGui::DockBuilderDockWindow("Logs", bottom);
                ImGui::DockBuilderDockWindow("Content Browser", bottom);
                ImGui::DockBuilderDockWindow("Properties", right);
                ImGui::DockBuilderDockWindow("Inspector", left);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }
    }

    void CreateWindows()
    {
        CreateSceneHierarchyWindow();
        CreateObjectDetailsWindow();
        CreateSceneWindow();
        CreateInspectorWindow();
        CreateLogsWindow();

        // Демо окна
        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        if (showAnotherWindow)
            CreateAnotherWindow();

        if (showAnimationWindow)
            CreateAnimationWindow();
    }

    void CreateSceneHierarchyWindow()
    {
        ImGui::Begin("Scene Hierarchy");
        if (ImGui::TreeNode("Game Objects"))
        {
            if (ImGui::TreeNode("Player"))
            {
                ImGui::Text("Transform");
                ImGui::Text("Sprite Renderer");
                ImGui::Text("Rigidbody");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Enemies"))
            {
                ImGui::Text("Enemy1");
                ImGui::Text("Enemy2");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Environment"))
            {
                ImGui::Text("Ground");
                ImGui::Text("Walls");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void CreateObjectDetailsWindow()
    {
        ImGui::Begin("Object Details");
        ImGui::Text("Transform Component");
        ImGui::DragFloat3("Position", &floatValue, 0.1f);
        ImGui::DragFloat3("Rotation", &floatValue, 0.1f);
        ImGui::DragFloat3("Scale", &floatValue, 0.1f);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Sprite Renderer");
        ImGui::Checkbox("Enabled", &boolValue);
        ImGui::ColorEdit4("Color", (float*)&clearColor);
        ImGui::Combo("Sprite", &currentItem, items, IM_ARRAYSIZE(items));
        ImGui::SliderFloat("Opacity", &floatValue, 0.0f, 1.0f);
        ImGui::End();
    }

    void CreateSceneWindow()
    {
        ImGui::Begin("Scene");
        ImGui::Text("Game View - %dx%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);

        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Фон
        drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
            IM_COL32(50, 50, 50, 255));

        // Сетка
        for (int i = 0; i < canvasSize.x; i += 20)
            drawList->AddLine(ImVec2(canvasPos.x + i, canvasPos.y),
                ImVec2(canvasPos.x + i, canvasPos.y + canvasSize.y),
                IM_COL32(70, 70, 70, 100));
        for (int i = 0; i < canvasSize.y; i += 20)
            drawList->AddLine(ImVec2(canvasPos.x, canvasPos.y + i),
                ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + i),
                IM_COL32(70, 70, 70, 100));

        // Прямоугольник (игровой объект)
        drawList->AddRectFilled(ImVec2(canvasPos.x + 100, canvasPos.y + 100),
            ImVec2(canvasPos.x + 150, canvasPos.y + 150),
            IM_COL32(255, 100, 100, 255));
        drawList->AddRect(ImVec2(canvasPos.x + 100, canvasPos.y + 100),
            ImVec2(canvasPos.x + 150, canvasPos.y + 150),
            IM_COL32(255, 255, 255, 255));
        ImGui::End();
    }

    void CreateInspectorWindow()
    {
        ImGui::Begin("Inspector");
        ImGui::Text("Selected: Player");
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Position", &floatValue);
            ImGui::InputFloat3("Rotation", &floatValue);
            ImGui::InputFloat3("Scale", &floatValue);
        }
        if (ImGui::CollapsingHeader("Components"))
        {
            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("component_popup");
            }

            if (ImGui::BeginPopup("component_popup"))
            {
                if (ImGui::MenuItem("Sprite Renderer")) {}
                if (ImGui::MenuItem("Rigidbody")) {}
                if (ImGui::MenuItem("Collider")) {}
                if (ImGui::MenuItem("Script")) {}
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void CreateLogsWindow()
    {
        ImGui::Begin("Logs");
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "[INFO] Application started");
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "[WARNING] Low memory");
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "[ERROR] Failed to load asset");
        ImGui::Text("Frame time: %.2f ms", ImGui::GetIO().DeltaTime * 1000.0f);
        ImGui::End();
    }

    void CreateAnotherWindow()
    {
        static bool firstDock = true;

        if (firstDock)
        {
            // Находим ID док-узла правой панели
            ImGuiWindow* objectDetailsWindow = ImGui::FindWindowByName("Object Details");
            if (objectDetailsWindow && objectDetailsWindow->DockNode)
            {
                ImGui::SetNextWindowDockID(objectDetailsWindow->DockNode->ID);
            }
            firstDock = false;
        }

        ImGui::Begin("Another Window", &showAnotherWindow);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            showAnotherWindow = false;
        ImGui::End();
    }

    void CreateAnimationWindow()
    {
        static bool firstDock = true;

        if (firstDock)
        {
            // Находим ID док-узла правой панели
            ImGuiWindow* objectDetailsWindow = ImGui::FindWindowByName("Object Details");
            if (objectDetailsWindow && objectDetailsWindow->DockNode)
            {
                ImGui::SetNextWindowDockID(objectDetailsWindow->DockNode->ID);
            }
            firstDock = false;
        }

        animation_editor.gui();

    }

    void SetUIScale(float scale)
    {
        ImGui::GetIO().FontGlobalScale = scale;
    }
};

int main()
{
    EditorApplication app;
    app.Run();
    return 0;
}