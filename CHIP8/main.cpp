#include "Emulator.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger("log_file");
    GameDisplay game_display("CHIP_8 emulator", 800, 600, &logger);

    Emulator emulator(&logger, &game_display);
    emulator.run_program();

    while(game_display.isOpen()){
        emulator.update();
        game_display.handleEvents();

        game_display.display();
    }

    return 0;
}