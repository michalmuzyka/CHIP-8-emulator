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
    void draw_roms_list();

    std::vector<std::filesystem::path> roms;
    int selected_rom = 0;

    void update();

    Emulator* emulator;
    Keyboard* keyboard;
    Logger* logger;
    Settings* settings;
    DebuggerWindow window;
};