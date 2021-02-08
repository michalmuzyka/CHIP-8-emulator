#include "Opcode.hpp"

Opcode::Opcode(const Opcode& c) {
    for (int i = 0; i < SIZE; ++i)
        hex_chars[i] = c.hex_chars[i];
}

void Opcode::create_from_binary(const unsigned char opcode[2]) {
    hex_chars[0] = (opcode[0] & 0b11110000) >> 4;
    hex_chars[1] = opcode[0] & 0b1111;
    hex_chars[2] = (opcode[1] & 0b11110000) >> 4;
    hex_chars[3] = opcode[1] & 0b1111;

    execute = [](Devices* dev) {};

}