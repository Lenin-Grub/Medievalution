#include "LogDisplay.hpp"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <Common/IconText.h>
#include <chrono>
#include <ctime>

std::vector<LogEntry> LogDisplaySink::get_logs() const
{
    std::lock_guard<std::mutex> lock(logs_mutex);
    return logs;
}

void LogDisplaySink::clear_logs()
{
    std::lock_guard<std::mutex> lock(logs_mutex);
    logs.clear();
}

void LogDisplaySink::sink_it_(const spdlog::details::log_msg& msg)
{
    LogEntry entry;
    entry.message = fmt::to_string(msg.payload);
    entry.level = msg.level;
    
    auto time_t = std::chrono::system_clock::to_time_t(msg.time);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(msg.time.time_since_epoch()) % 1000;
    
    std::tm tm;
    localtime_s(&tm, &time_t);
    
    char timestamp_buffer[64];
    std::strftime(timestamp_buffer, sizeof(timestamp_buffer), "%H:%M:%S", &tm);
    entry.timestamp = fmt::format("{}.{:03d}", timestamp_buffer, ms.count());
    
    {
        std::lock_guard<std::mutex> lock(logs_mutex);
        logs.push_back(entry);
        
        if (logs.size() > max_logs)
        {
            logs.erase(logs.begin());
        }
    }
}

void LogDisplaySink::flush_()
{
    // do nothing
}

LogDisplay::LogDisplay()
{
    log_sink = std::make_shared<LogDisplaySink>();
    
    auto logger = spdlog::default_logger();
    logger->sinks().push_back(log_sink);
}

void LogDisplay::draw()
{
    ImGui::Begin(SET_ICON_TEXT((Icon::SCRIPT), " Logs"));
    
    if (ImGui::Button("Clear"))
    {
        log_sink->clear_logs();
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &auto_scroll);
    
    ImGui::SameLine();
    ImGui::Checkbox("Show timestamps", &show_timestamps);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    const char* level_names[] = { "All", "Debug", "Info", "Warn", "Error", "Critical" };
    ImGui::Combo("Level", &level_filter, level_names, IM_ARRAYSIZE(level_names));
    
    ImGui::Separator();
    
    if (ImGui::BeginChild("LogArea", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
    {
        auto logs = log_sink->get_logs();
        
        for (const auto& entry : logs)
        {
            if (level_filter > 0 && static_cast<int>(entry.level) < level_filter - 1)
                continue;
                
            drawLog(entry);
        }
        
        if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::EndChild();
    
    ImGui::End();
}

void LogDisplay::update(const float& delta_time)
{
}

void LogDisplay::drawLog(const LogEntry& entry)
{
    ImVec4 color = getLevelColor(entry.level);
    
    if (show_timestamps)
    {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[%s]", entry.timestamp.c_str());
        ImGui::SameLine();
    }
    
    ImGui::TextColored(color, "[%s]", getLevelName(entry.level));
    ImGui::SameLine();
    ImGui::TextWrapped("%s", entry.message.c_str());
}

ImVec4 LogDisplay::getLevelColor(spdlog::level::level_enum level)
{
    switch (level)
    {
        case spdlog::level::debug:
            return ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray
        case spdlog::level::info:
            return ImVec4(0.5f, 1.0f, 0.5f, 1.0f); // Green
        case spdlog::level::warn:
            return ImVec4(1.0f, 1.0f, 0.5f, 1.0f); // Yellow
        case spdlog::level::err:
            return ImVec4(1.0f, 0.5f, 0.5f, 1.0f); // Red
        case spdlog::level::critical:
            return ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Light Red
        default:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
    }
}

const char* LogDisplay::getLevelName(spdlog::level::level_enum level)
{
    switch (level)
    {
        case spdlog::level::debug:    return "DEBUG";
        case spdlog::level::info:     return "INFO";
        case spdlog::level::warn:     return "WARN";
        case spdlog::level::err:      return "ERROR";
        case spdlog::level::critical: return "CRITICAL";
        default:                      return "UNKNOWN";
    }
}
