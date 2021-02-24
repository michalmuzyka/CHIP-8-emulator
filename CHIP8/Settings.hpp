#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Color.hpp>

#include "Logger.hpp"

class Settings
{
public:
    Settings(const std::string& path, Logger* logger);
    std::string operator[](const std::string& string);

    sf::Color get_color(const std::string& string);
    int get_int(const std::string& string);
    float get_float(const std::string& string);
private:
    std::map<std::string, std::string> settings;
    Logger* logger;

    void parse_config_file(const std::string& path);
};