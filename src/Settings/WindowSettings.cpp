﻿#include "../stdafx.h"
#include "WindowSettings.h"

// если settings.json не обнаружено, то будут приняты эти настройки
WindowSettings::WindowSettings():
    title("Uninitialized"),
    resolution(sf::VideoMode::getDesktopMode()),
    video_modes(sf::VideoMode::getFullscreenModes()),
    fullscreen(false),
    vertical_sync(false),
    fps_limit(0),
    music_volume(100),
    sound_volume(100),
    camera_speed(1),
    zoom_speed(0),
    language("rus"),
    is_sound_play(true),
    is_music_play(true),
    id_resolution(0)
{
    context_settings.depthBits         = 24;
    context_settings.stencilBits       = 8;
    context_settings.antialiasingLevel = 4;
    context_settings.majorVersion      = 3;
    context_settings.minorVersion      = 0;
}

bool WindowSettings::saveToFile(const std::string path) noexcept
{
    nlohmann::ordered_json j;
    std::ofstream file("config/settings.json");
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        //TODO json ругается на синхронизацию, т.к. она int, а требуется bool
        j["title"]         = title;
        j["resolution"]    = { { "x", resolution.width }, { "y", resolution.height } };
        j["id_resolution"] = id_resolution;
        j["fullscreen"]    = fullscreen;
        j["fps_limit"]     = fps_limit;
        j["music_volume"]  = music_volume;
        j["camera_speed"]  = camera_speed;
        j["zoom_speed"]    = zoom_speed;
        j["language"]      = language;
        file << std::setw(4) << j;
        ofs.close();
        LOG_INFO("Settings\t Saved");
        return true;
    }
    return false;
}

bool WindowSettings::loadFromFIle(const std::string path) noexcept
{
    nlohmann::json j;
    std::ifstream ifs(path);

    const std::vector<std::string> requiredKeys = { "title", "resolution", "id_resolution", "fullscreen", "fps_limit", "music_volume", "camera_speed", "zoom_speed", "language" };
    if (!ifs.is_open())
    {
        LOG_ERROR("Settings.json not found!");
        return false;
    }

    j = nlohmann::json::parse(ifs);
    for (const auto& key : requiredKeys)
    {
        if (!j.contains(key))
        {
            LOG_ERROR("Uncorrected key in settings file. Please check it.");
            return false;
        }
    }
    //TODO json ругается на синхронизацию, т.к. она int, а требуется bool
    j["title"].get_to(title);
    j["resolution"]["x"].get_to(resolution.width);
    j["resolution"]["y"].get_to(resolution.height);
    j["id_resolution"].get_to(id_resolution);
    j["fullscreen"].get_to(fullscreen);
    j["fps_limit"].get_to(fps_limit);
    j["music_volume"].get_to(music_volume);
    j["camera_speed"].get_to(camera_speed);
    j["zoom_speed"].get_to(zoom_speed);
    j["language"].get_to(language);
    ifs.close();
    LOG_INFO("Settings\t Init");
    return true;
}
