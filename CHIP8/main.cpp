#include <iostream>

#include "Emulator.hpp"

int main()
{
    Logger logger("log_file");
    Keyboard keyboard;
    Emulator emulator(&logger, &keyboard);

    while(emulator.window.is_open()){

        emulator.window.handle_events();
        emulator.window.display();

        emulator.update();
    }

    return 0;
}