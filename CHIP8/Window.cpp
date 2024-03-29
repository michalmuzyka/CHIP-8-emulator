#include "Window.hpp"
#include <SFML/Window/Event.hpp>

namespace CHIP8
{

    void Window::open(const std::string& window_title, const unsigned& window_width, const unsigned& window_height)
    {
        window.create(sf::VideoMode{ window_width, window_height }, window_title, sf::Style::Close);
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(300);

        if (!window.isOpen())
            log(LOG_ERROR, "SFML: Error, unable to open a window");
    }

    void Window::handle_events() {
        if (!is_open()) return;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
                if (keyboard) keyboard->handle_keyboard_events(event);
        }

        if (keyboard && keyboard->is_pressed(sf::Keyboard::Escape))
            window.close();
    }

    void Window::clear(sf::Color color) {
        window.clear(color);
    }

    Window::~Window() {
        window.close();
    }

    bool Window::is_open() const {
        return window.isOpen();
    }

    void Window::link_keyboard(Keyboard* keyboard) {
        this->keyboard = keyboard;
    }


    GameWindow::GameWindow(sf::Vector2i display_pixel_size)
        :display_pixel_size{ display_pixel_size },
        drawn_pixels(display_pixel_size.y, std::vector<unsigned char>(display_pixel_size.x, 0))
    {
        const int p_size = Settings::ins()->get_int("pixel_size");
        pixel_size = sf::Vector2i{ p_size, p_size };
        pixel.setSize(sf::Vector2f(p_size, p_size));

        pixel_color = Settings::ins()->get_color("pixel_color");
        background_color = Settings::ins()->get_color("background_color");

        board_texture.create(pixel_size.x * display_pixel_size.x, pixel_size.y * display_pixel_size.y);
        game_board.setPosition(0, 0);
        board_texture.clear(background_color);

        if (Settings::ins()->get_int("play_sound_effect") && !buzz_buffer.loadFromFile("buzz.wav"))
            log(LOG_ERROR, "Emulator: Cannot load buzzer sound");

        buzz_sound.setBuffer(buzz_buffer);
        buzz_sound.setLoop(false);
        buzz_sound.setVolume(Settings::ins()->get_float("volume"));
    }

    void GameWindow::open() {
        const int width = pixel_size.x * display_pixel_size.x;
        const int height = pixel_size.y * display_pixel_size.y;

        Window::open("CHIP_8 emulation", width, height);

        const auto current_pos = window.getPosition();
        window.setPosition(sf::Vector2i{ current_pos.x + width / 2, current_pos.y });
    }

    void GameWindow::clear() {
        board_texture.clear(background_color);
        for (auto& row : drawn_pixels)
            row = std::vector<unsigned char>(display_pixel_size.x, 0);
    }

    bool GameWindow::draw_pixels_row(sf::Vector2i at, const unsigned char& row) {
        unsigned char y = at.y % display_pixel_size.y;
        bool pixel_erased = false;
        unsigned char mask = 0b10000000;

        for (int i = 0; i < 8; ++i) {
            unsigned char bit = (row & mask) >> (CHAR_BIT - 1 - i); // char is 8bit long
            unsigned char x = (at.x + i) % display_pixel_size.x;

            if (drawn_pixels[y][x] == 1 && bit == 0)
                pixel_erased = true;

            drawn_pixels[y][x] ^= bit;

            if (drawn_pixels[y][x])
                pixel.setFillColor(pixel_color);
            else
                pixel.setFillColor(background_color);
            pixel.setPosition(x * pixel_size.x, y * pixel_size.y);
            board_texture.draw(pixel);

            mask >>= 1;
        }

        return pixel_erased;
    }

    void GameWindow::play_buzzer() {
        if (Settings::ins()->get_int("play_sound_effect"))
            buzz_sound.play();
    }

    void GameWindow::display() {
        if (!is_open()) return;

        window.clear(background_color);
        board_texture.display();
        game_board.setTexture(board_texture.getTexture());
        window.draw(game_board);
        window.display();
    }
}