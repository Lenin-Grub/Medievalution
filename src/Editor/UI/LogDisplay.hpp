#pragma once
#include <UI/IDisplay.hpp>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <vector>
#include <string>
#include <mutex>

struct ImVec4;

struct LogEntry
{
    std::string message;
    spdlog::level::level_enum level { spdlog::level::info };
    std::string timestamp;
};

class LogDisplaySink 
    : public spdlog::sinks::base_sink<std::mutex>
{
public:
    LogDisplaySink() = default;
    
    std::vector<LogEntry> get_logs() const;
    void clear_logs();

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

private:
    std::vector<LogEntry> logs;
    mutable std::mutex logs_mutex;
    static constexpr size_t max_logs = 1000;
};

class LogDisplay 
    : public UI::IDisplay
{
public:
    LogDisplay();
    virtual ~LogDisplay() = default;

    void draw() override;
    void update(const float& delta_time) override;
    
private:

    void drawLog(const LogEntry& entry);
    ImVec4 getLevelColor(spdlog::level::level_enum level);
    const char* getLevelName(spdlog::level::level_enum level);

private:
    std::shared_ptr<LogDisplaySink> log_sink;
    bool auto_scroll     = true;
    bool show_timestamps = true;
    bool show_debug      = true;
    bool show_info       = true;
    bool show_warn       = true;
    bool show_error      = true;
    bool show_critical   = true;
    char search_query[256] = "";
};
