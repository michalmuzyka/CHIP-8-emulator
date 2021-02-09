#pragma once
#include <string>
#include "Logger.hpp"
#include "Window.hpp"
#include "Keyboard.hpp"
#include "Opcode.hpp"

class Emulator
{
public:
    Emulator(Logger* logger, GameWindow* display, Keyboard* keyboard);

    bool load_program_from_file(const std::string &path);
    void execute_current_line();
    void update();

private:
    static const unsigned ROM_SIZE{ 2048 };
    Opcode program[ROM_SIZE];
    unsigned instruction_count{ 0 };

    Devices devices;
};
