#include "Display.hpp"

Display::Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger)
    :logger{ logger }
{
    window.create(sf::VideoMode{ window_width, window_height }, window_title);

    if (logger && !window.isOpen())
        logger->log(MESSAGE_TYPE::ERROR, "Error, unable to open a window");
}

Display::~Display() {
    window.close();
}