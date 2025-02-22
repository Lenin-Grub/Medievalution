#include "stdafx.h"
#include "WindowSettings.h"

// если settings.json не обнаружено, то будут приняты эти настройки
WindowSettings::WindowSettings()
    : fps(50)
    , lastFrameTime (common::clock.getElapsedTime())
{
    settings.title                              = "Uninitialized";
    settings.resolution                         = sf::VideoMode::getDesktopMode();
    settings.video_modes                        = sf::VideoMode::getFullscreenModes();
    settings.fullscreen                         = false;
    settings.vertical_sync                      = false;
    settings.fps_limit                          = 0;
    settings.music_volume                       = 100;
    settings.sound_volume                       = 100;
    settings.camera_speed                       = 1;
    settings.zoom_speed                         = 0;
    settings.language                           = "rus";
    settings.is_sound_play                      = true;
    settings.is_music_play                      = true;
    settings.id_resolution                      = 0;

    settings.context_settings.depthBits         = 24;
    settings.context_settings.stencilBits       = 8;
    settings.context_settings.antialiasingLevel = 4;
    settings.context_settings.majorVersion      = 3;
    settings.context_settings.minorVersion      = 0;
}

bool WindowSettings::saveToFile(const std::string path) noexcept
{
    nlohmann::ordered_json j;
    std::ofstream file("config/settings.json");
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        //TODO json ругается на синхронизацию, т.к. она int, а требуется bool
        j["title"]         = settings.title;
        j["resolution"]    = { { "x", settings.resolution.width }, { "y", settings.resolution.height } };
        j["id_resolution"] = settings.id_resolution;
        j["fullscreen"]    = settings.fullscreen;
        j["fps_limit"]     = settings.fps_limit;
        j["music_volume"]  = settings.music_volume;
        j["camera_speed"]  = settings.camera_speed;
        j["zoom_speed"]    = settings.zoom_speed;
        j["language"]      = settings.language;
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
    j["title"].get_to          (settings.title);
    j["resolution"]["x"].get_to(settings.resolution.width);
    j["resolution"]["y"].get_to(settings.resolution.height);
    j["id_resolution"].get_to  (settings.id_resolution);
    j["fullscreen"].get_to     (settings.fullscreen);
    j["fps_limit"].get_to      (settings.fps_limit);
    j["music_volume"].get_to   (settings.music_volume);
    j["camera_speed"].get_to   (settings.camera_speed);
    j["zoom_speed"].get_to     (settings.zoom_speed);
    j["language"].get_to       (settings.language);
    ifs.close();
    LOG_INFO("Settings\t Init");
    return true;
}


float WindowSettings::getFPS()
{
    sf::Time currentTime = common::clock.getElapsedTime();
    float timeDifference = currentTime.asSeconds() - lastFrameTime.asSeconds();

    if (timeDifference > 0)
        fps = 1.0f / timeDifference;
    else
        fps = 0.0f;

    lastFrameTime = currentTime;
    return fps;
}