#include <iostream>

#include "Emulator.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger("log_file");

    Keyboard keyboard;

    GameWindow game_window("CHIP_8 emulator", 800, 600, &logger);
    game_window.link_keyboard(&keyboard);

    Emulator emulator(&logger, &game_window, &keyboard);
    emulator.run_program();

    while(game_window.is_open()){
        game_window.handle_events();
        game_window.display();

        emulator.update();
    }

    return 0;
}