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
    unsigned I{ 0 };
    unsigned PC{ 0 };
    unsigned char delay_timer{ 0 };
    unsigned char sound_timer{ 0 };
    std::stack<int> stack;

    bool PC_should_be_increment{true};

    GameWindow* window;
    Keyboard* keyboard;
    Logger* logger;
};

class Opcode
{
public:
    Opcode() = default;

    void create_from_binary(const unsigned char opcode[2]);

    std::function<void(Devices*)> execute;
private:
    static int get_PC_address_from_binary(const unsigned char hex_chars[4]);

};