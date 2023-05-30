#include "../stdafx.h"
#include "../State/SettingsState.h"

//------------------------------------------------------------------------------------------------------------------------
// констуктор
SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	this->updateEvents();
}

//------------------------------------------------------------------------------------------------------------------------
// деструктор
SettingsState::~SettingsState()
{
}

//------------------------------------------------------------------------------------------------------------------------
// обновление событий
void SettingsState::updateEvents()
{
}

//------------------------------------------------------------------------------------------------------------------------
// обновление IMGUI
void SettingsState::updateImGui()
{
	const char *resolution[] = { "1920x1080", "1289x720", "1024x768", "800x600", "640x480" };
	//#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Settings##", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (ImGui::BeginCombo("Resolution", *resolution))
	{
		if (ImGui::Button(resolution[0], ImVec2(120, 0)))
		{
			stateData->graphicSettings->resolution.width = 1920;
			stateData->graphicSettings->resolution.height = 1080;
		}
		if (ImGui::Button(resolution[1], ImVec2(120, 0)))
		{
			stateData->graphicSettings->resolution.width = 1280;
			stateData->graphicSettings->resolution.height = 720;
		}
		if (ImGui::Button(resolution[2], ImVec2(120, 0)))
		{
			stateData->graphicSettings->resolution.width = 1024;
			stateData->graphicSettings->resolution.height = 768;
		}
		if (ImGui::Button(resolution[3], ImVec2(120, 0)))
		{
			stateData->graphicSettings->resolution.width = 800;
			stateData->graphicSettings->resolution.height = 600;
		}
		if (ImGui::Button(resolution[4], ImVec2(120, 0)))
		{
			stateData->graphicSettings->resolution.width = 640;
			stateData->graphicSettings->resolution.height = 480;
		}
		ImGui::EndCombo();
	}

	if (ImGui::Checkbox("Full Window", &this->stateData->graphicSettings->fullscrean))
	{
		this->stateData->graphicSettings->fullscrean;
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::SetTooltip("Set windowed mode\nOr set full resolution mode");
		ImGui::EndTooltip();
	}

	if (ImGui::Checkbox("Vertycal sinhronization", &this->stateData->graphicSettings->vertycalSync))
	{
		this->stateData->graphicSettings->vertycalSync;
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::SetTooltip("Set vertycal sinhronization\nIs off by default");
		ImGui::EndTooltip();
	}

	ImGui::InputInt("FPS limit", &this->stateData->graphicSettings->fps_limit, 0,1);

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::SetTooltip("Set fps to 0 for unlimited fps \nOr set any other value");
		ImGui::EndTooltip();
	}

	ImGui::Separator();
	{
		ImGui::SliderInt("Sound Volume", &stateData->graphicSettings->sound_volume, 0, 100, "%d%%");
	}
	ImGui::SameLine();
	{
		ImGui::Checkbox("Sound", &play_sound);
	}

	if (ImGui::SliderInt("Music Volume", &stateData->graphicSettings->music_volume, 0, 100, "%d%%"))
	{
		core::music->setVolume(stateData->graphicSettings->music_volume);
	}

	ImGui::SameLine();
	{
		ImGui::Checkbox("Music", &play_music);
	}
	ImGui::Separator();
	{
		int i = 1;
		ImGui::SliderInt("Zoom speed", &i, 1, 100, "%d%%");
	}
	{
		int i = 1;
		ImGui::SliderInt("Camera speed", &stateData->graphicSettings->camera_speed, 1, 100, "%d%%");
	}

	if (ImGui::Button("Apllay"))
		ImGui::OpenPopup("Applay?");

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Applay?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("You changed game settings.\n" "All unsaving settings will be lost.\n""Save it?\n\n");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{
			LOG_INFO("Settings changed");
			this->stateData->graphicSettings->saveToFile("config/graphic_settings.ini");
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit##"))
	{
		StateManager::getInstance().endState();
		//endState();
	}
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	ImGui::Begin("T1", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

//------------------------------------------------------------------------------------------------------------------------
//обновляем все
void SettingsState::update(const float& dtime)									
{
	this->updateMousePositions();
}

//------------------------------------------------------------------------------------------------------------------------
//рисуем все
void SettingsState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(core::view);													// установлена камера вида
	target->draw(this->mouseCordsText);											// координаты

}
//------------------------------------------------------------------------------------------------------------------------