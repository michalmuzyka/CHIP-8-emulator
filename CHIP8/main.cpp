#include "Emulator.hpp"

int main()
{
    Logger logger("log_file");

    Keyboard keyboard;

    GameWindow game_window("CHIP_8 emulator", sf::Vector2i{ 8,8 }, sf::Vector2i{ 64, 32 }, &logger);
    game_window.link_keyboard(&keyboard);

    Emulator emulator(&logger, &game_window, &keyboard);

    while(game_window.is_open()){
        game_window.handle_events();
        game_window.display();

        emulator.update();
    }

    return 0;
}