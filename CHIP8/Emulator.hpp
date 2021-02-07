#pragma once
#include <stack>
#include <string>
#include "Logger.hpp"
#include "Window.hpp"
#include "Keyboard.hpp"
#include "Parser.hpp"

class Emulator
{
public:
    Emulator(Logger* logger, Window* display, Keyboard* keyboard);

    bool load_program_from_file(const std::string &path);
    void run_program();
    void update();

private:
    static const unsigned ROM_SIZE = 2048;
    static const unsigned RAM_SIZE = 4096;

    unsigned char RAM[RAM_SIZE] {0};
    Command ROM[ROM_SIZE];
    unsigned char V[16] {0};
    unsigned char I[2] {0};
    unsigned char PC[2] {0};
    unsigned char delay_timer {0};
    unsigned char sound_timer {0};
    std::stack<char[2]> stack;

    Logger *logger;
    Window *window;
    Keyboard *keyboard;

    void execute_command();
};
