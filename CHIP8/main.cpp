#include <iostream>
#include "Debugger.hpp"

int main()
{
    Logger logger("log_file");
    Settings settings("config.ini", &logger);
    Keyboard keyboard;
    Emulator emulator(&logger, &settings, &keyboard);
    Debugger debugger(&emulator);
    debugger.start_emulation();

    return 0;
}