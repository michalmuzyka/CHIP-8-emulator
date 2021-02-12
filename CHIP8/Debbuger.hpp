#pragma once
#include "Emulator.hpp"

class Debbuger
{
public:
    Debbuger();

private:
    Emulator* emulator;
    Logger* logger;
    DebuggerWindow* window;
};