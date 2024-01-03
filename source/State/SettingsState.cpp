#include "../stdafx.h"
#include "../State/SettingsState.h"
#include "SettingsState.h"

SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	this->updateEvents();
	if (!background.loadFromFile("resources/Backgrounds/background.jpg"))
	{
		LOG_ERROR("File <<background.jpg>> not foubd");
	}

	shape.setSize(vec2f(StateManager::getInstance().stateData.window->getSize().x, StateManager::getInstance().stateData.window->getSize().y));
	shape.setTexture(&background);

	//TODO
	video_modes = sf::VideoMode::getFullscreenModes();
	StateManager::getInstance().addObserver(*this);

	for (int i = 0; i < WindowSettings::getInstance().resolution.getFullscreenModes().size(); ++i)
	{
		res[i] = WindowSettings::getInstance().resolution.getFullscreenModes().at(i);
		LOG_INFO("{0},{1}", WindowSettings::getInstance().resolution.getFullscreenModes().at(i).width, WindowSettings::getInstance().resolution.getFullscreenModes().at(i).height);
	}
	item_current_idx = WindowSettings::getInstance().id_resolution;
}

SettingsState::~SettingsState()
{
	StateManager::getInstance().removeObserver(*this);
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
	const char* resolution[] =
	{
	"3840x2160","2560x1600", "2560x1440", "2048x1536", "1920x1440","1920x1200", "1920x1080",
	"1680x1050", "1600x1200", "1600x1024", "1600x900", "1440x1080","1440x900", "1366x768",
	"1360x768", "1280x1024", "1280x960", "1280x800", "1280x768", "1280x720", "1176x664",
	"1152x864", "1024x768" ,"800x600", "720x576", "720x480", "640x480"
	};




	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	std::string str = WindowSettings::getInstance().localisation.at("T_settings");
	ImGui::Begin(str.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);


	
	const char* combo_preview_value = resolution[item_current_idx];

	str = WindowSettings::getInstance().localisation.at("T_resolution");
	if (ImGui::BeginCombo(str.c_str(), combo_preview_value,0))
	{
		for (int n = 0; n < ((int)(sizeof(resolution) / sizeof(*(resolution)))); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(resolution[n], is_selected))
			{
				//TODO добавить функцию изменения разрешения экрана
				item_current_idx = n;
				WindowSettings::getInstance().id_resolution = item_current_idx;
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
	ImGui::BeginDisabled();
	if (ImGui::Checkbox(str.c_str(), &WindowSettings::getInstance().vertycalSync))
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
		core::music->setVolume(WindowSettings::getInstance().music_volume);
	}

	ImGui::SameLine();
	{
		str = WindowSettings::getInstance().localisation.at("T_music");
		ImGui::Checkbox(str.c_str(), &play_music);
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

void SettingsState::update(const float& dtime)									
{
	this->updateMousePositions();
}

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