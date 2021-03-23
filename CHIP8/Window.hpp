#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Logger.hpp"
#include "Keyboard.hpp"
#include "Settings.hpp"

class Window
{
public:
    Window(Settings* settings);
    void open(const std::string& window_title, const unsigned& window_width, const unsigned& window_height);
    virtual ~Window() = default;

    void clear(sf::Color color);
    virtual void handle_events() final;
    virtual void display() = 0;
    virtual bool is_open() const final;
    virtual void link_keyboard(Keyboard* keyboard) final;

protected:
    sf::RenderWindow window;

    Settings* settings;
    Keyboard* keyboard = nullptr;
};

class GameWindow :public Window
{
public:
    GameWindow(const std::string& window_title, sf::Vector2i display_pixel_size, Settings* settings);

    void open();
    void clear();
    bool draw_pixels_row(sf::Vector2i at, const unsigned char& row);
    void display() override;
    void play_buzzer();

private:
    sf::SoundBuffer buzz_buffer;
    sf::Sound buzz_sound;

    sf::Sprite game_board;
    sf::RenderTexture board_texture;
    const sf::Vector2i display_pixel_size;
    sf::Color background_color;

    sf::RectangleShape pixel;
    sf::Vector2i pixel_size;
    sf::Color pixel_color;

    std::vector<std::vector<unsigned char>> drawn_pixels;
};

class DebuggerWindow :public Window
{
public:
   DebuggerWindow(Settings* settings);
   void display() override;
   void draw_text(const std::string &text, sf::Vector2i at, bool with_outline = false);
   void open();

private:
    sf::Font font;
    sf::Text text;
};