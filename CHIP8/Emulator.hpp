#pragma once
#include <stack>
#include <string>
#include <random>
#include "Logger.hpp"
#include "Window.hpp"
#include "Keyboard.hpp"

class Emulator
{
public:
    Emulator(Logger* logger, GameWindow* display, Keyboard* keyboard);

    bool load_program_from_file(const std::string& path);
    void execute_current_line();
    void update();

private:
    unsigned last_instruction_addr{ 0x200 };

    static const unsigned RAM_SIZE = 4096;
    unsigned char RAM[RAM_SIZE]{ 0 };
    unsigned char V[16]{ 0 };
    unsigned I{ 0 };
    unsigned PC{ 0x200 };
    unsigned char delay_timer{ 0 };
    unsigned char sound_timer{ 0 };
    std::stack<int> stack;

    std::mt19937 generator;
    bool PC_should_be_increment{ true };

    Logger* logger;
    GameWindow* window;
    Keyboard* keyboard;

    static int get_address_from_binary(const unsigned char hex_chars[4]);
    static unsigned char get_constant_from_binary(const unsigned char hex_chars[4]);
};
