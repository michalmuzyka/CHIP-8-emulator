#pragma once
#include <string>

enum class MESSAGE_TYPE
{
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    Logger(const std::string &log_file_path);
    void log(MESSAGE_TYPE type, const std::string& message) const;
private:
    std::string log_file_path;
};
