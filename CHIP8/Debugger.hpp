#pragma once
#include "Emulator.hpp"

class Debugger
{
public:
    Debugger(Emulator* emulator);
    void start_emulation();

private:
    Emulator* emulator;
    Keyboard* keyboard;
    Logger* logger;
    DebuggerWindow window;
};