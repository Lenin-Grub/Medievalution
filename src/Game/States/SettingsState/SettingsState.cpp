#include "SettingsState.hpp"
#include <Localisation/Localisation.hpp>

SettingsState::SettingsState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State      { data, machine, window, replace }
, play_music { true }
, play_sound { true }
, video_modes(sf::VideoMode::getFullscreenModes())
, resolution_current_id(WindowSettings::getInstance().settings.id_resolution)
{
    state_machine.is_init = true;
    initial_settings      = WindowSettings::getInstance().settings;
}

void SettingsState::init()
{
    setBackground();
    LOG_INFO("State Settings\t Init"); 
}

void SettingsState::onDeactivate()
{
    LOG_INFO("State Settings\t Deactivate");

}

void SettingsState::onActivate()
{
    LOG_INFO("State Settings\t Activate");
}

void SettingsState::updateEvents()
{
    if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        state_machine.lastState();
    }
}

void SettingsState::updateImGui()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin(LOCLIZATION(ICON::getStr(Icon::SETTINGS), "T_settings"), nullptr, ImGuiWindowFlags_NoCollapse
                                                                                                             | ImGuiWindowFlags_AlwaysAutoResize
                                                                                                             | ImGuiWindowFlags_NoMove);

    renderWindowSettings();
    renderSoundsAndMusic();
    renderCameraSettings();
    renderApplySettings();
    renderExitAndMetrics();

    ImGui::End();
}

void SettingsState::update(const float& dtime)
{
}

void SettingsState::draw(sf::RenderTarget* target)
{
    if (!target)
        target = &window;
    target->setView(window.getDefaultView());
    window.draw(shape);
    ImGui::SFML::Render(window);
}

void SettingsState::setBackground()
{
    background = ResourceLoader::instance().getTexture("background_3.jpg");
    shape.setTexture(&background);
    shape.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
}



void SettingsState::renderWindowSettings() 
{
    std::string str_preview = std::to_string(video_modes.at(resolution_current_id).width) + "x" +
                              std::to_string(video_modes.at(resolution_current_id).height);

    const char* combo_preview_value = str_preview.c_str();

    if (ImGui::BeginCombo(LOCLIZATION(ICON::getStr(Icon::MONITOR), "T_resolution"), combo_preview_value, 0))
    {
        for (int n = 0; n < video_modes.size(); n++) 
        {
            const bool is_selected = (resolution_current_id == n);

            std::string string_all_resolutions = std::to_string(video_modes.at(n).width) + " x " +
                                                 std::to_string(video_modes.at(n).height);

            auto modes = string_all_resolutions.c_str();

            if (ImGui::Selectable(modes, is_selected)) 
            {
                resolution_current_id = n;
                WindowSettings::getInstance().settings.id_resolution = resolution_current_id;
                WindowSettings::getInstance().settings.resolution.width = video_modes.at(n).width;
                WindowSettings::getInstance().settings.resolution.height = video_modes.at(n).height;
            }

            if (is_selected) 
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Checkbox(LOCLIZATION(ICON::getStr(Icon::MONITOR), "T_full_window"), &WindowSettings::getInstance().settings.fullscreen))
    {
        WindowSettings::getInstance().settings.fullscreen;
    }

    if (ImGui::IsItemHovered()) 
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(LOCLIZATION("T_full_window_tooltip"));
        ImGui::EndTooltip();
    }

    ImGui::BeginDisabled();
    ImGui::Checkbox(LOCLIZATION("T_vertical_sync"), &WindowSettings::getInstance().settings.vertical_sync);
    ImGui::EndDisabled();

    if (ImGui::IsItemHovered()) 
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(LOCLIZATION("T_vertical_sync_tooltip"));
        ImGui::EndTooltip();
    }

    ImGui::InputInt(LOCLIZATION("T_fps_limit"), &WindowSettings::getInstance().settings.fps_limit, 0, 1);

    if (ImGui::IsItemHovered()) 
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(LOCLIZATION("T_fps_tooltip"));
        ImGui::EndTooltip();
    }
}

void SettingsState::renderSoundsAndMusic() 
{
    ImGui::Separator();

    ImGui::BeginTable("table1", 2);
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::SliderInt(LOCLIZATION("T_sound_volume"), &WindowSettings::getInstance().settings.sound_volume, 0, 100, "%d%%");
    ImGui::TableNextColumn();

    ImGui::Checkbox(LOCLIZATION("T_sound"), &play_sound);
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (ImGui::SliderInt(LOCLIZATION("T_music_volume"), &WindowSettings::getInstance().settings.music_volume, 0, 100, "%d%%"))
        state_machine.data.jukebox.setVolume((int)WindowSettings::getInstance().settings.music_volume);
    ImGui::TableNextColumn();

    if (ImGui::Checkbox(LOCLIZATION("T_music"), &play_music))
    {
        if (play_music) 
            state_machine.data.jukebox.play();
        else 
            state_machine.data.jukebox.pause();
    }
    ImGui::EndTable();
}

void SettingsState::renderCameraSettings() 
{
    ImGui::Separator();
    ImGui::BeginDisabled();
    ImGui::SliderFloat(LOCLIZATION("T_zoom_speed"), &WindowSettings::getInstance().settings.zoom_speed, 0, 1, "%.1f");
    ImGui::SliderInt(LOCLIZATION("T_camera_speed"), &WindowSettings::getInstance().settings.camera_speed, 1, 100, "%d%%");
    ImGui::EndDisabled();
}

void SettingsState::renderApplySettings() 
{
    ImGui::Separator();
    if (ImGui::Button(LOCLIZATION("T_apply")))
    {
        ImGui::OpenPopup(LOCLIZATION("T_apply"));
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    bool settings_changed = false;
    if (initial_settings != WindowSettings::getInstance().settings) 
        settings_changed = true;

    if (ImGui::BeginPopupModal(LOCLIZATION("T_apply"), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(LOCLIZATION("T_changed_game_settings"));
        ImGui::Separator();

        if (ImGui::Button(LOCLIZATION("T_yes"), ImVec2(120, 0)))
        {
            if (settings_changed) 
            {
                if (initial_settings.resolution != WindowSettings::getInstance().settings.resolution ||
                    initial_settings.language   != WindowSettings::getInstance().settings.language) 
                {
                    state_machine.restart(true);
                }

                WindowSettings::getInstance().saveToFile("config/settings.json");
                LOG_INFO("Settings\t Changed");
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button(LOCLIZATION("T_cancel"), ImVec2(120, 0)))
        {
            WindowSettings::getInstance().settings = initial_settings;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void SettingsState::renderExitAndMetrics() 
{
    ImGui::SameLine();
    if (ImGui::Button(LOCLIZATION("T_exit")))
        state_machine.lastState();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T1", nullptr, ImGuiWindowFlags_NoDecoration
                              | ImGuiWindowFlags_AlwaysAutoResize
                              | ImGuiWindowFlags_NoFocusOnAppearing
                              | ImGuiWindowFlags_NoNav);

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    ImGui::End();
}
