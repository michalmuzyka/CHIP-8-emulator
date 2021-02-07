#pragma once

struct Command
{
    unsigned id;

};

class Parser
{
public:
    static Command parse(unsigned char opcode[2]);
};