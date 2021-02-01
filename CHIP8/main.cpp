#include "Emulator.hpp"
#include "Logger.h"

int main()
{
    Logger logger("log");
    Emulator emulator;
    emulator.link_logger(&logger);



    return 0;
}