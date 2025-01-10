#include "stdafx.h"
#include "StateMachine.hpp"
#include "SettingsState.hpp"


SettingsState::SettingsState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State      { data, machine, window, replace }
, play_music { true }
, play_sound { true }
, video_modes(sf::VideoMode::getFullscreenModes())
, resolution_current_id(WindowSettings::getInstance().id_resolution)
{
    state_machine.is_init = true;
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
    ImGui::Begin(Localization::getInstance().get(ICON_SETTINGS, "T_settings").c_str(), nullptr, ImGuiWindowFlags_NoCollapse
                                                              | ImGuiWindowFlags_AlwaysAutoResize 
                                                              | ImGuiWindowFlags_NoMove);

#pragma region Window settings
    std::string str_preview = std::to_string(video_modes.at(resolution_current_id).width) + "x" +
                              std::to_string(video_modes.at(resolution_current_id).height);


    const char* combo_preview_value = str_preview.c_str();

    if (ImGui::BeginCombo(Localization::getInstance().get(ICON_MONITOR, "T_resolution").c_str(), combo_preview_value, 0))
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
                WindowSettings::getInstance().id_resolution		= resolution_current_id;
                WindowSettings::getInstance().resolution.width	= video_modes.at(n).width;
                WindowSettings::getInstance().resolution.height = video_modes.at(n).height;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    //------------------------------------------------------------------------------------
    if (ImGui::Checkbox(Localization::getInstance().get(ICON_MONITOR, "T_full_window").c_str(), &WindowSettings::getInstance().fullscreen))
        WindowSettings::getInstance().fullscreen;

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(Localization::getInstance().get("T_full_window_tooltip").c_str());
        ImGui::EndTooltip();
    }

    ImGui::BeginDisabled();
    ImGui::Checkbox(Localization::getInstance().get("T_vertical_sync").c_str(), &WindowSettings::getInstance().vertical_sync);
    ImGui::EndDisabled();

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(Localization::getInstance().get("T_vertical_sync_tooltip").c_str());
        ImGui::EndTooltip();
    }

    ImGui::InputInt(Localization::getInstance().get("T_fps_limit").c_str(), &WindowSettings::getInstance().fps_limit, 0, 1);

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::SetTooltip(Localization::getInstance().get("T_fps_tooltip").c_str());
        ImGui::EndTooltip();
    }
#pragma endregion

#pragma region Sounds and Musics
    //------------------------------------------------------------------------------------
    ImGui::Separator();
    ImGui::BeginTable("table1", 2);
    ImGui::TableNextRow();
    //------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    ImGui::SliderInt(Localization::getInstance().get("T_sound_volume").c_str(), &WindowSettings::getInstance().sound_volume, 0, 100, "%d%%");
    //------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    ImGui::Checkbox(Localization::getInstance().get("T_sound").c_str(), &play_sound);
    //------------------------------------------------------------------------------------
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    if (ImGui::SliderInt(Localization::getInstance().get("T_music_volume").c_str(), &WindowSettings::getInstance().music_volume, 0, 100, "%d%%"))
        state_machine.data.jukebox.setVolume((int)WindowSettings::getInstance().music_volume);
    //------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
     if (ImGui::Checkbox(Localization::getInstance().get("T_music").c_str(), &play_music))
     {
         if (play_music)
             state_machine.data.jukebox.play();
         else
             state_machine.data.jukebox.pause();
     }
     ImGui::EndTable();
#pragma endregion

#pragma region Camera settings 
    ImGui::Separator();
    ImGui::BeginDisabled();
    ImGui::SliderFloat(Localization::getInstance().get("T_zoom_speed").c_str(), &WindowSettings::getInstance().zoom_speed, 0, 1, "%.1f");
    ImGui::SliderInt(Localization::getInstance().get("T_camera_speed").c_str(), &WindowSettings::getInstance().camera_speed, 1, 100, "%d%%");
    ImGui::EndDisabled();
#pragma endregion

#pragma region Apply 
    ImGui::Separator();
    if (ImGui::Button(Localization::getInstance().get("T_apply").c_str()))
    {
        ImGui::OpenPopup(Localization::getInstance().get("T_apply").c_str());
    }
#pragma endregion

#pragma region Apply  settings
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(Localization::getInstance().get("T_apply").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(Localization::getInstance().get("T_changed_game_settings").c_str());
        ImGui::Separator();

        if (ImGui::Button(Localization::getInstance().get("T_yes").c_str(), ImVec2(120, 0)))
        {
            state_machine.restart(true);
            WindowSettings::getInstance().saveToFile("config/settings.json");
            ImGui::CloseCurrentPopup();
            LOG_INFO("Settings\t Changed");
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button(Localization::getInstance().get("T_cancel").c_str(), ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
#pragma endregion

#pragma region Exit and metrics

    ImGui::SameLine();
    if (ImGui::Button(Localization::getInstance().get("T_exit").c_str()))
    {
        state_machine.lastState();
    }
    ImGui::End();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("T1", nullptr, ImGuiWindowFlags_NoDecoration 
                              | ImGuiWindowFlags_AlwaysAutoResize 
                              | ImGuiWindowFlags_NoFocusOnAppearing 
                              | ImGuiWindowFlags_NoNav);

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, WindowSettings::getInstance().getFPS());
    ImGui::End();
#pragma endregion
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
