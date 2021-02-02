#include "Emulator.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

Emulator::Emulator(Logger* logger, Display* display)
    :logger{ logger }, display{ display }
{
    load_program_from_file("test_opcode.ch8");
}

bool Emulator::load_program_from_file(const std::string& path) {
    std::ifstream program_file(path, std::ios::in | std::ios::binary);

    if (!program_file.is_open()) {
        if(logger) logger->log(MESSAGE_TYPE::ERROR, "Can't open program file");
        return false;
    }

    int instruction_counter = 0;

    while(program_file.read(reinterpret_cast<char*>(ROM[instruction_counter]), 2)){
        ++instruction_counter;

        if (instruction_counter == ROM_SIZE) {
            memset(ROM, 0, ROM_SIZE*2);
            if (logger) logger->log(MESSAGE_TYPE::ERROR, "File is too big to load to ROM");
            return false;
        }
    }

    return true;
}

void Emulator::run_program() {

}

void Emulator::update() {
    delay_timer = delay_timer ? delay_timer - 1 : 0;
    sound_timer = sound_timer ? sound_timer - 1 : 0;
}