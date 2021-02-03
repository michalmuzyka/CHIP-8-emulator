#include "Window.hpp"
#include <SFML/Window/Event.hpp>

Window::Window(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger)
    :logger{ logger } {
    window.create(sf::VideoMode{ window_width, window_height }, window_title);

    if (logger && !window.isOpen())
        logger->log(MESSAGE_TYPE::ERROR, "Error, unable to open a window");
}

void Window::handle_events() {
    sf::Event event;
    while(window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

bool Window::is_open() const {
    return window.isOpen();
}

void Window::link_keyboard(Keyboard *keyboard) {
    this->keyboard = keyboard;
}

GameWindow::GameWindow(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger)
    :Window{ window_title, window_width, window_height, logger } {
}

void GameWindow::display() {
    window.clear(sf::Color::Black);


    window.display();
}