#include "Settings.hpp"
#include <filesystem>
#include <fstream>

Settings::Settings(const std::string& path, Logger* logger)
    :logger(logger)
{
    parse_config_file(path);
}

void Settings::parse_config_file(const std::string& path) {
    namespace fs = std::filesystem;
    if (!fs::exists(path))
        logger->log(MESSAGE_TYPE::ERROR, path + " doesn't exist");
    if (!fs::is_regular_file(path))
        logger->log(MESSAGE_TYPE::ERROR, path + " is not a file");

    std::ifstream config_file(path);

    if (!config_file.is_open())
        logger->log(MESSAGE_TYPE::ERROR, path + " cannot be opened");

    std::string line;
    while (std::getline(config_file, line)) {
        if (line[0] == '#') continue; // comment

        auto equation_pos = line.find('=');
        if (equation_pos == std::string::npos || equation_pos == line.length()-1)
            settings.insert({ line, "yes" });
        else {
            std::string key = line.substr(0, equation_pos);
            std::string value = line.substr(equation_pos + 1);
            settings.insert({ key, value });
        }
    }
}

std::string Settings::operator[](const std::string& string) {
    if (settings.find(string) == settings.end())
        return "no";

    return settings[string];
}

sf::Color Settings::get_color(const std::string& string) {
    std::string color = (*this)[string];
    if (color == "no")
        return sf::Color::White;

    sf::Uint8 r = std::stoi(color.substr(0, 2), nullptr, 16);
    sf::Uint8 g = std::stoi(color.substr(2, 2), nullptr, 16);
    sf::Uint8 b = std::stoi(color.substr(4, 2), nullptr, 16);

    return sf::Color{ r,g,b,255 };
}

int Settings::get_int(const std::string& string) {
    std::string number = (*this)[string];
    if (number == "no")
        return 0;

    return std::stoi(number);
}

float Settings::get_float(const std::string& string) {
    std::string number = (*this)[string];
    if (number == "no")
        return 0;

    return std::stof(number);
}
