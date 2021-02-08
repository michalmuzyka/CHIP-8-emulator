#pragma once
#include <stack>
#include <functional>
#include "Window.hpp"
#include "Keyboard.hpp"

struct Devices
{
    static const unsigned RAM_SIZE = 4096;
    unsigned char RAM[RAM_SIZE]{ 0 };
    unsigned char V[16]{ 0 };
    unsigned char I[2]{ 0 };
    unsigned char PC[2]{ 0 };
    unsigned char delay_timer{ 0 };
    unsigned char sound_timer{ 0 };
    std::stack<char[2]> stack;

    Window* window;
    Keyboard* keyboard;
    Logger* logger;
};

class Opcode{
public:
    std::function<void(Devices*)> execute;
    void create_from_binary(const unsigned char opcode[2]);
private:
    Opcode() = default;
    virtual ~Opcode() = default;
    Opcode(const Opcode & c);

    static const unsigned SIZE = 4;
    unsigned char hex_chars[SIZE];
};