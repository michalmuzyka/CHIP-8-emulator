#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Display
{
public:
    Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height);
    ~Display();

private:
    sf::RenderWindow window;

};