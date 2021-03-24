#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cstdlib>

enum class MESSAGE_TYPE
{
    INFO,
    WARNING,
    ERROR
};

static void log(MESSAGE_TYPE type, const std::string& message, std::string logfile_pathname = "log_file"){
    std::ostream* out_stream;

#ifdef _DEBUG
    out_stream = &std::clog;
#else
    std::ofstream logfile(name, std::ios::out | std::ios::app);
    if (!logfile.is_open()) return;
    out_stream = &logfile;
#endif

    const auto now = std::chrono::system_clock::now();
    auto now_in_time_t = std::chrono::system_clock::to_time_t(now);
    tm time_tm;
    localtime_s(&time_tm, &now_in_time_t);
    *out_stream << '[' << std::put_time(&time_tm, "%H:%M %d.%m.%Y") << "] ";
    switch (type) {
    case MESSAGE_TYPE::INFO: *out_stream << "INFO:"; break;
    case MESSAGE_TYPE::WARNING: *out_stream << "WARNING:"; break;
    case MESSAGE_TYPE::ERROR: *out_stream << "ERROR:"; break;
    }
    *out_stream << message << std::endl;

    if (type == MESSAGE_TYPE::ERROR)
        exit(1);
}
