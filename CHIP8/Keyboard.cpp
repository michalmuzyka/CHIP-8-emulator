#include "Keyboard.hpp"

void Keyboard::handle_keyboard_events(sf::Event event) {
     pressed_keys[event.key.code] = (event.type == sf::Event::KeyPressed);
}

bool Keyboard::is_pressed(sf::Keyboard::Key symbol) {
    return pressed_keys[symbol];
}
