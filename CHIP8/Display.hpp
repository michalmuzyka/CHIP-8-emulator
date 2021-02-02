#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Logger.hpp"

class Display
{
public:
    Display(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger);
    virtual ~Display() = default;

    virtual void handleEvents() final;
    virtual void display() = 0;
    virtual bool isOpen() const final;

protected:
    sf::RenderWindow window;
    Logger* logger;
};

class GameDisplay :public Display
{
public:
    GameDisplay(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger);

    void display() override;
};