#include "Window.hpp"
#include <SFML/Window/Event.hpp>

Window::Window(const std::string& window_title, const unsigned& window_width, const unsigned& window_height, Logger* log)
    :logger{ log }
{
    window.create(sf::VideoMode{ window_width, window_height }, window_title);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    if (logger && !window.isOpen())
        logger->log(MESSAGE_TYPE::ERROR, "Error, unable to open a window");
}

void Window::handle_events() {
    sf::Event event;
    while(window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
            if (keyboard) keyboard->handle_keyboard_events(event);
    }
}

bool Window::is_open() const {
    return window.isOpen();
}

void Window::link_keyboard(Keyboard *keyboard) {
    this->keyboard = keyboard;
}

GameWindow::GameWindow(const std::string& window_title, sf::Vector2i pixel_size, sf::Vector2i display_pixel_size,  Logger* log)
    :Window{ window_title, pixel_size.x * display_pixel_size.x, pixel_size.y * display_pixel_size.y, log },
     display_pixel_size{ display_pixel_size },
     pixel(sf::Vector2f{ pixel_size }),
     pixel_size{ pixel_size },
     pixels_drew( display_pixel_size.y, std::vector<bool>(display_pixel_size.x, false))
{
    board_texture.create(pixel_size.x * display_pixel_size.x, pixel_size.y * display_pixel_size.y);
    board_texture.setSmooth(false);
    board_texture.setRepeated(false);
    game_board.setPosition(0, 0);
    board_texture.clear(background_color);
}

void GameWindow::clear() {
    board_texture.clear(background_color);

    for(auto& row : pixels_drew)
        row = std::vector<bool>(display_pixel_size.x, false);
}

bool GameWindow::draw_pixels_row(sf::Vector2i at, const unsigned char &row) {
    bool pixel_changed = false;




}

void GameWindow::display() {
    window.clear(sf::Color::Black);
    game_board.setTexture(board_texture.getTexture());
    window.draw(game_board);
    window.display();
}