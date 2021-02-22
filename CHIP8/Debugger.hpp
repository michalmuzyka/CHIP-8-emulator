#pragma once
#include "Emulator.hpp"
#include "Settings.hpp"
#include <filesystem>

class Debugger
{
public:
    Debugger(Emulator* emulator);
    void start_emulation();

private:
    std::string roms_path;
    void scan_roms(const std::string& directory_path);

    std::vector<std::filesystem::path> roms;

    Emulator* emulator;
    Keyboard* keyboard;
    Logger* logger;
    Settings* settings;
    DebuggerWindow window;
};