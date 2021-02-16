#include "Window.hpp"
#include <SFML/Window/Event.hpp>

Window::Window(Logger* log)
    :logger{log} {   
}

void Window::open(const std::string& window_title, const unsigned& window_width, const unsigned& window_height)
{
    window.create(sf::VideoMode{ window_width, window_height }, window_title, sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(500);

    if (logger && !window.isOpen())
        logger->log(MESSAGE_TYPE::ERROR, "Error, unable to open a window");
}

void Window::handle_events() {
    if(!is_open()) return;

    sf::Event event;
    while(window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            if (keyboard) keyboard->handle_keyboard_events(event);
    }

    if (keyboard && keyboard->is_pressed(sf::Keyboard::Escape))
        window.close();
}

bool Window::is_open() const {
    return window.isOpen();
}

void Window::link_keyboard(Keyboard *keyboard) {
    this->keyboard = keyboard;
}

GameWindow::GameWindow(const std::string& window_title, sf::Vector2i pixel_size, sf::Vector2i display_pixel_size,  Logger* log)
    :Window{ log },
     display_pixel_size{ display_pixel_size },
     pixel(sf::Vector2f{ pixel_size }),
     pixel_size{ pixel_size },
     drawn_pixels( display_pixel_size.y, std::vector<unsigned char>(display_pixel_size.x, 0))
{
    board_texture.create(pixel_size.x * display_pixel_size.x, pixel_size.y * display_pixel_size.y);
    board_texture.setSmooth(false);
    board_texture.setRepeated(false);
    game_board.setPosition(0, 0);
    board_texture.clear(background_color);

    if (!buzz_buffer.loadFromFile("buzz.wav"))
        logger->log(MESSAGE_TYPE::ERROR, "CAN'T LOAD BUZZER SOUND");
    buzz_sound.setBuffer(buzz_buffer);
    buzz_sound.setLoop(false);
}

void GameWindow::open() {
    Window::open("CHIP_8 emulator", pixel_size.x * display_pixel_size.x, pixel_size.y * display_pixel_size.y);
}

void GameWindow::clear() {
    board_texture.clear(background_color);
    for(auto& row : drawn_pixels)
        row = std::vector<unsigned char>(display_pixel_size.x, 0);
}

bool GameWindow::draw_pixels_row(sf::Vector2i at, const unsigned char &row) {
    unsigned char y = at.y % display_pixel_size.y;
    bool pixel_erased = false;
    unsigned char mask = 0b10000000;
    
    for (int i = 0; i < 8; ++i) {
        unsigned char bit = (row & mask) >> (8 - 1 - i); // char is 8bit long
        unsigned char x = (at.x + i) % display_pixel_size.x;

        if (drawn_pixels[y][x] == 1 && bit == 0)
            pixel_erased = true;

        drawn_pixels[y][x] ^= bit;

        if (drawn_pixels[y][x])
            pixel.setFillColor(pixel_color);
        else
            pixel.setFillColor(background_color);
        pixel.setPosition(x*pixel_size.x, y*pixel_size.y);
        board_texture.draw(pixel);

        mask >>= 1;
    }

    return pixel_erased;
}

void GameWindow::play_buzzer() {
    buzz_sound.play();
}

void GameWindow::display() {
    if (!is_open()) return;

    window.clear(sf::Color::Black);
    board_texture.display();
    game_board.setTexture(board_texture.getTexture());
    window.draw(game_board);
    window.display();
}


DebuggerWindow::DebuggerWindow() {
    
}

void DebuggerWindow::display() {
    
}

