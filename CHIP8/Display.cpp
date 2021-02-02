#include "Display.hpp"

Display::Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height) {
    window.create(sf::VideoMode{ window_width, window_height }, window_title);
}

Display::~Display() {
    window.close();
}
