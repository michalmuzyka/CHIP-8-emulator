#pragma once
#include "Emulator.hpp"
#include <filesystem>

class Debugger
{
public:
    Debugger(Emulator* emulator);
    void start_emulation();

private:
    std::string roms_path = "ROMS";
    void scan_roms(std::string path);

    std::vector<std::filesystem::path> roms;

    Emulator* emulator;
    Keyboard* keyboard;
    Logger* logger;
   // DebuggerWindow window;
};