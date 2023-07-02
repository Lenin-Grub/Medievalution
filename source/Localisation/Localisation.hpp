#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"

class Localisation
{
public:
	static Localisation& getInstance()
	{
		static Localisation single_instance;
		return single_instance;
	}

	std::string getStringByKey(std::string key);
private:
	nlohmann::json eng {
        {"T_applay", "true"},
        {"T_camera_speed" , "Camera speed"},
        {"T_cancel" , "Cancel"},
        {"T_changed_game_settings" , "You change game settings\nUnsaved settings will be lost\nSave it?"},
        {"T_exit", "Exit"},
        {"T_fps_limit" , "FPS limit"},
        {"T_fps_tooltip" , "Set0 for unlimited fps\nOr set another value"},
        {"T_full_window" , "Full screan"},
        {"T_full_window_tooltip" , "On/off window mode"},
        {"T_music" , "Music"},
        {"T_music_volume" , "Musics volume"},
        {"T_new_game" , "New game"},
        {"T_resolution" , "Resolution"},
        {"T_settings" , "Settings"},
        {"T_sound" , "Sound"},
        {"T_sound_volume" , "Sound volume"},
        {"T_vertychal_sync" , "Vertuchal syncronization"},
        {"T_vertychal_sync_tooltip" , "Vertuchal syncronization\nOff by default"},
        {"T_yes" , "Yes"},
        {"T_zoom_speed" , "Zoom speed"}
    };
};