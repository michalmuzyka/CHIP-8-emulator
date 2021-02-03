#pragma once
#include <unordered_map>
#include <SFML/Window/Event.hpp>

class Keyboard
{
public:
    void handle_keyboard_events(sf::Event event);
    bool is_pressed(sf::Keyboard::Key symbol);
private:
    std::unordered_map<sf::Keyboard::Key, bool> pressed_keys;
};