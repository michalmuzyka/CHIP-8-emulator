#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Color.hpp>
#include "Logger.hpp"

namespace CHIP8
{

    class Settings
    {
    public:
        static std::string settings_path;

        static Settings* ins();
        ~Settings();

        std::string operator[](const std::string& string);
        sf::Color get_color(const std::string& string);
        int get_int(const std::string& string);
        float get_float(const std::string& string);

        Settings(Settings&) = delete;
        Settings operator=(Settings&) = delete;

    private:
        static Settings* instance;

        Settings(const std::string& path);
        std::map<std::string, std::string> settings;
        void parse_config_file(const std::string& path);
    };

}