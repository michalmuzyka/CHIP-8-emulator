#include "Display.hpp"
#include <SFML/Window/Event.hpp>

Display::Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger)
    :logger{ logger } {
    window.create(sf::VideoMode{ window_width, window_height }, window_title);

    if (logger && !window.isOpen())
        logger->log(MESSAGE_TYPE::ERROR, "Error, unable to open a window");
}

void Display::handleEvents() {
    sf::Event event;
    while(window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

bool Display::isOpen() const {
    return window.isOpen();
}

GameDisplay::GameDisplay(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger)
    :Display(window_title, window_width, window_height, logger) {
}

void GameDisplay::display() {
    window.clear(sf::Color::Black);


    window.display();
}