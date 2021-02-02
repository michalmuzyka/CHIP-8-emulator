#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Logger.hpp"

class Display
{
public:
    Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger);
    ~Display();

private:
    sf::RenderWindow window;

    Logger* logger;
};