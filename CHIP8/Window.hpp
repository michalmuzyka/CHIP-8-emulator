#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Logger.hpp"
#include "Keyboard.hpp"

class Window
{
public:
    Window(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger);
    virtual ~Window() = default;

    virtual void handle_events() final;
    virtual void display() = 0;
    virtual bool is_open() const final;
    virtual void link_keyboard(Keyboard* keyboard) final;

protected:
    sf::RenderWindow window;

    Keyboard* keyboard = nullptr;
    Logger* logger;
};

class GameWindow :public Window
{
public:
    GameWindow(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* logger);

    void display() override;
};