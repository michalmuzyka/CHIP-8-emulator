#include <iostream>
#include "Debugger.hpp"

int main()
{
    Settings settings("config.ini");
    Keyboard keyboard;
    Emulator emulator(&settings, &keyboard);
    Debugger debugger(&emulator);
    debugger.start_emulation();

    return 0;
}