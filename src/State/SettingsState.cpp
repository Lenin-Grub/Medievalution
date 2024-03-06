﻿#include "../stdafx.h"
#include "../State/SettingsState.h"
#include "SettingsState.h"

SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
	,video_modes(sf::VideoMode::getFullscreenModes())
	,resolution_current_id(WindowSettings::getInstance().id_resolution)
{
	StateManager::getInstance().addObserver(*this);
	setBackground();
}

SettingsState::~SettingsState()
{
	StateManager::getInstance().removeObserver(*this);
}

void SettingsState::setBackground()
{
	if (!background.loadFromFile("resources/Backgrounds/background.jpg"))
	{
		LOG_ERROR("File <<background.jpg>> not foubd");
	}

	shape.setSize(vec2f(StateManager::getInstance().state_data.window->getSize().x, StateManager::getInstance().state_data.window->getSize().y));
	shape.setTexture(&background);
}


void SettingsState::updateEvents(){}

void SettingsState::updateImGui()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	std::string str = WindowSettings::getInstance().localisation.at("T_settings");

	ImGui::Begin(((char*)ICON_SETTINGS + str).c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoMove);
	
	//std::string string_resolution = std::to_string(video_modes.at(resolution_current_id).width) + " x " + std::to_string(video_modes.at(resolution_current_id).height);
	//auto char_resolution = string_resolution.c_str();
	auto str2 = std::to_string((video_modes.at(resolution_current_id).width)) + "x" + std::to_string(video_modes.at(resolution_current_id).height);
	const char* combo_preview_value = str2.c_str();

	str = WindowSettings::getInstance().localisation.at("T_resolution");
	if (ImGui::BeginCombo(str.c_str(), combo_preview_value,0))
	{
		for (int n = 0; n < video_modes.size(); n++)
		{
			const bool is_selected = (resolution_current_id == n);

			std::string string_all_resolutions = std::to_string(video_modes.at(n).width) + " x " + std::to_string(video_modes.at(n).height);
			auto modes = string_all_resolutions.c_str();

			if (ImGui::Selectable(modes, is_selected))
			{
				//TODO добавить функцию изменения разрешения экрана
				resolution_current_id = n;
				WindowSettings::getInstance().id_resolution = resolution_current_id;
				WindowSettings::getInstance().resolution.width = video_modes.at(n).width;
				WindowSettings::getInstance().resolution.height = video_modes.at(n).height;
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	str = WindowSettings::getInstance().localisation.at("T_full_window");
	if (ImGui::Checkbox(((char*)ICON_MAX_SIEZE + str).c_str(), &WindowSettings::getInstance().fullscrean))
	{
		WindowSettings::getInstance().fullscrean;
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		str = WindowSettings::getInstance().localisation.at("T_full_window_tooltip");
		ImGui::SetTooltip(str.c_str());
		ImGui::EndTooltip();
	}

	str = WindowSettings::getInstance().localisation.at("T_vertychal_sync");
	ImGui::BeginDisabled();
	if (ImGui::Checkbox(str.c_str(), &WindowSettings::getInstance().vertycal_sync))
	{

	}
	ImGui::EndDisabled();

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		str = WindowSettings::getInstance().localisation.at("T_vertychal_sync_tooltip");
		ImGui::SetTooltip(str.c_str());
		ImGui::EndTooltip();
	}

	str = WindowSettings::getInstance().localisation.at("T_fps_limit");
	ImGui::InputInt(str.c_str(), &WindowSettings::getInstance().fps_limit, 0,1);

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		str = WindowSettings::getInstance().localisation.at("T_fps_tooltip");
		ImGui::SetTooltip(str.c_str());
		ImGui::EndTooltip();
	}

	ImGui::Separator();
	{
		str = WindowSettings::getInstance().localisation.at("T_sound_volume");
		ImGui::SliderInt(str.c_str(), &WindowSettings::getInstance().sound_volume, 0, 100, "%d%%");
	}
	ImGui::SameLine();
	{
		str = WindowSettings::getInstance().localisation.at("T_sound");
		ImGui::Checkbox(str.c_str(), &play_sound);
	}

	str = WindowSettings::getInstance().localisation.at("T_music_volume");
	if (ImGui::SliderInt(str.c_str(), &WindowSettings::getInstance().music_volume, 0, 100, "%d%%"))
	{
		StateManager::getInstance().state_data.jukebox.setVolume((int)WindowSettings::getInstance().music_volume);
	}

	ImGui::SameLine();
	{
		str = WindowSettings::getInstance().localisation.at("T_music");
		if (ImGui::Checkbox(str.c_str(), &play_music))
		{
			if (play_music)
			{
				StateManager::getInstance().state_data.jukebox.play();
			}
			else
			{
				StateManager::getInstance().state_data.jukebox.pause();
			}
		}
	}
	ImGui::Separator();
	ImGui::BeginDisabled();
	{
		str = WindowSettings::getInstance().localisation.at("T_zoom_speed");
		ImGui::SliderFloat(str.c_str(),&WindowSettings::getInstance().zoom_speed, 0, 1,"%.1f");
	}
	{
		str = WindowSettings::getInstance().localisation.at("T_camera_speed");
		ImGui::SliderInt(str.c_str(), &WindowSettings::getInstance().camera_speed, 1, 100, "%d%%");
	}
	ImGui::EndDisabled();
	ImGui::Separator();
	
	str = WindowSettings::getInstance().localisation.at("T_applay");
	if (ImGui::Button(str.c_str()))
	{
		str = WindowSettings::getInstance().localisation.at("T_applay");
		ImGui::OpenPopup(str.c_str());
	}

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	str = WindowSettings::getInstance().localisation.at("T_applay");
	if (ImGui::BeginPopupModal(str.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		str = WindowSettings::getInstance().localisation.at("T_changed_game_settings");
		ImGui::Text(str.c_str());
		ImGui::Separator();

		str = WindowSettings::getInstance().localisation.at("T_yes");
		if (ImGui::Button(str.c_str(), ImVec2(120, 0)))
		{
			LOG_INFO("Settings changed");
			WindowSettings::getInstance().saveToFile("config/settings.json");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		str = WindowSettings::getInstance().localisation.at("T_cancel");
		if (ImGui::Button(str.c_str(), ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	str = WindowSettings::getInstance().localisation.at("T_exit");
	if (ImGui::Button(str.c_str()))
	{
		StateManager::getInstance().endState();
	}
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T1", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void SettingsState::update(const float& dtime){updateMousePositions();}

void SettingsState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(this->window->getDefaultView());
	target->draw(shape);
}

void SettingsState::updateObserver()
{
	LOG_INFO("observer settings update");
}