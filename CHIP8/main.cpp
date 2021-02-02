#include "Emulator.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger("log_file");
    Display display("CHIP_8 emulator", 800, 600, &logger);

    Emulator emulator(&logger, &display);
    emulator.run_program();

    return 0;
}