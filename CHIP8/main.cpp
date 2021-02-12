#include <iostream>
#include "Debugger.hpp"

int main()
{
    Logger logger("log_file");
    Keyboard keyboard;
    Emulator emulator(&logger, &keyboard);
    Debugger debugger(&emulator);
    debugger.start_emulation();

    return 0;
}