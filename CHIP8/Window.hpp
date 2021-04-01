#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Keyboard.hpp"
#include "Settings.hpp"

namespace CHIP8
{
    class Window
    {
    public:
        void open(const std::string& window_title, const unsigned& window_width, const unsigned& window_height);
        virtual ~Window();

        void clear(sf::Color color);
        virtual void handle_events() final;
        virtual void display() = 0;
        virtual bool is_open() const final;
        virtual void link_keyboard(Keyboard* keyboard) final;

    protected:
        sf::RenderWindow window;
        Keyboard* keyboard = nullptr;
    };

    class GameWindow :public Window
    {
    public:
        GameWindow(const std::string& window_title, sf::Vector2i display_pixel_size);

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
}