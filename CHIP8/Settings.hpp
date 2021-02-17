#pragma once
#include <string>
#include <map>
#include "Logger.hpp"

class Settings
{
public:
    Settings(const std::string& path, Logger* logger);
    std::string operator[](const std::string& string);

private:
    std::map<std::string, std::string> settings;

    void parse_config_file(const std::string& path);

    Logger* logger;
};

