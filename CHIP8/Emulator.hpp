#pragma once
#include <string>
#include "Logger.hpp"
#include "Window.hpp"
#include "Keyboard.hpp"
#include "Opcode.hpp"

class Emulator
{
public:
    Emulator(Logger* logger, Window* display, Keyboard* keyboard);

    bool load_program_from_file(const std::string &path);
    void run_program();
    void update();

private:
    static const unsigned ROM_SIZE = 2048;
    std::unique_ptr<Opcode> program[ROM_SIZE];

    Devices devices;
};
