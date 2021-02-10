#include "Keyboard.hpp"

void Keyboard::handle_keyboard_events(sf::Event event) {
     pressed_keys[event.key.code] = (event.type == sf::Event::KeyPressed);
}

bool Keyboard::is_pressed(sf::Keyboard::Key symbol) {
    return pressed_keys[symbol];
}

bool Keyboard::CHIP8_key_is_pressed(int symbol) {
    /*
     * KEYBOARD BINDING:
     * chip8:    keyboard:
     * 1 2 3 C   1 2 3 4
     * 4 5 6 D   Q W E R
     * 7 8 9 E   A S D F
     * A 0 B F   Z X C V
     */

    switch(symbol){
        case 0x0: return is_pressed(sf::Keyboard::X);
        case 0x1: return is_pressed(sf::Keyboard::Num1);
        case 0x2: return is_pressed(sf::Keyboard::Num2);
        case 0x3: return is_pressed(sf::Keyboard::Num3);
        case 0x4: return is_pressed(sf::Keyboard::Q);
        case 0x5: return is_pressed(sf::Keyboard::W);
        case 0x6: return is_pressed(sf::Keyboard::E);
        case 0x7: return is_pressed(sf::Keyboard::A);
        case 0x8: return is_pressed(sf::Keyboard::S);
        case 0x9: return is_pressed(sf::Keyboard::D);
        case 0xA: return is_pressed(sf::Keyboard::Z);
        case 0xB: return is_pressed(sf::Keyboard::C);
        case 0xC: return is_pressed(sf::Keyboard::Num4);
        case 0xD: return is_pressed(sf::Keyboard::R);
        case 0xE: return is_pressed(sf::Keyboard::F);
        case 0xF: return is_pressed(sf::Keyboard::V);
        default: return false;
    }
}

bool Keyboard::is_any_pressed() {
    bool pressed = false;
    for (unsigned i = 0x0; i <= 0xF; ++i)
        pressed |= CHIP8_key_is_pressed(i);
    return pressed;
}
