#include "Keyboard.hpp"

#include <iostream>

Keyboard::Keyboard() {
    //TODO    
}


void Keyboard::handle_keyboard_events(sf::Event event) {
     pressed_keys[event.key.code] = (event.type == sf::Event::KeyPressed);
}

bool Keyboard::isPressed(sf::Keyboard::Key symbol) {
    return pressed_keys[symbol];
}
