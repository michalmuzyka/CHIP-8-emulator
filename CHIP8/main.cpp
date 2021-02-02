#include "Emulator.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger("log_file");
    GameDisplay display("CHIP_8 emulator", 800, 600, &logger);

    Emulator emulator(&logger, &display);
    emulator.run_program();

    while(display.isOpen()){
        emulator.update();
        display.handleEvents();

        display.display();
    }

    return 0;
}