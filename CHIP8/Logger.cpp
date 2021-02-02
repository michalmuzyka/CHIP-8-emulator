#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cstdlib>

Logger::Logger(const std::string &log_file_path)
    :log_file_path{ log_file_path }
{
}

void Logger::log(MESSAGE_TYPE type, const std::string &message) const {
    std::ostream* out_stream;

#ifdef _DEBUG
    out_stream = &std::clog;
#else
    std::ofstream logfile(log_file_path, std::ios::out | std::ios::app);
    if (!logfile.is_open()) return;
    out_stream = &logfile;
#endif

    const auto now = std::chrono::system_clock::now();
    auto now_in_time_t = std::chrono::system_clock::to_time_t(now);
    tm time_tm;
    localtime_s(&time_tm, &now_in_time_t);
    *out_stream << '[' << std::put_time(&time_tm, "%H:%M %d.%m.%Y") << "] ";
    switch(type){
        case MESSAGE_TYPE::INFO: *out_stream << "INFO:"; break;
        case MESSAGE_TYPE::WARNING: *out_stream << "WARNING:"; break;
        case MESSAGE_TYPE::ERROR: *out_stream << "ERROR:"; break;
    }
    *out_stream << message << std::endl;

    if (type == MESSAGE_TYPE::ERROR)
        exit(1);
}