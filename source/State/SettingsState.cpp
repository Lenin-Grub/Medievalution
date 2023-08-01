#include "../stdafx.h"
#include "../State/SettingsState.h"
#include "SettingsState.h"

SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	this->updateEvents();
	background.loadFromFile("resources/background.jpg");
	sh.setSize(vec2f(StateManager::getInstance().stateData.window->getSize().x, StateManager::getInstance().stateData.window->getSize().y));
	sh.setTexture(&background);

	//TODO
	video_modes = sf::VideoMode::getFullscreenModes();
}

SettingsState::~SettingsState()
{
}

void SettingsState::updateEvents()
{
	if (core::sfmlEvent.type == sf::Event::KeyReleased && core::sfmlEvent.key.code == sf::Keyboard::D)
	{

		std::cout << "event work correct" << "\n";
	}
}

void SettingsState::updateImGui()
{
	const char *resolution[] = { "1920x1080", "1289x720", "1024x768", "800x600", "640x480" };
	//#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	std::string str = WindowSettings::getInstance().localisation.at("T_settings");
	ImGui::Begin(str.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	str = WindowSettings::getInstance().localisation.at("T_resolution");
	if (ImGui::BeginCombo(str.c_str(), *resolution))
	{
		if (ImGui::Button(resolution[0], ImVec2(120, 0)))
		{
			WindowSettings::getInstance().resolution.width = 1920;
			WindowSettings::getInstance().resolution.height = 1080;
		}
		if (ImGui::Button(resolution[1], ImVec2(120, 0)))
		{
			 WindowSettings::getInstance().resolution.width = 1280;
			 WindowSettings::getInstance().resolution.height = 720;
		}
		if (ImGui::Button(resolution[2], ImVec2(120, 0)))
		{
			 WindowSettings::getInstance().resolution.width = 1024;
			 WindowSettings::getInstance().resolution.height = 768;
		}
		if (ImGui::Button(resolution[3], ImVec2(120, 0)))
		{
			 WindowSettings::getInstance().resolution.width = 800;
			 WindowSettings::getInstance().resolution.height = 600;
		}
		if (ImGui::Button(resolution[4], ImVec2(120, 0)))
		{
			 WindowSettings::getInstance().resolution.width = 640;
			 WindowSettings::getInstance().resolution.height = 480;
		}
		ImGui::EndCombo();
	}

	str = WindowSettings::getInstance().localisation.at("T_full_window");
	if (ImGui::Checkbox(str.c_str(), &WindowSettings::getInstance().fullscrean))
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
	if (ImGui::Checkbox(str.c_str(), &WindowSettings::getInstance().vertycalSync))
	{
		WindowSettings::getInstance().vertycalSync;
	}

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
		core::music->setVolume(WindowSettings::getInstance().music_volume);
	}

	ImGui::SameLine();
	{
		str = WindowSettings::getInstance().localisation.at("T_music");
		ImGui::Checkbox(str.c_str(), &play_music);
	}
	ImGui::Separator();
	{
		str = WindowSettings::getInstance().localisation.at("T_zoom_speed");
		ImGui::SliderFloat(str.c_str(),&WindowSettings::getInstance().zoom_speed, 0, 1,"%.1f");
	}
	{
		str = WindowSettings::getInstance().localisation.at("T_camera_speed");
		ImGui::SliderInt(str.c_str(), &WindowSettings::getInstance().camera_speed, 1, 100, "%d%%");
	}
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

void SettingsState::update(const float& dtime)									
{
	this->updateMousePositions();
}

void SettingsState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(this->window->getDefaultView());
	target->draw(sh);																// координаты
}