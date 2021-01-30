#include "Emulator.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

Emulator::Emulator() {
    load_program_from_file("test_opcode.ch8");
}

bool Emulator::load_program_from_file(std::string path) {
    std::ifstream program_file(path, std::ios::in | std::ios::binary);

    if (!program_file.is_open())
        return false;

    int instruction_counter = 0;

    while(program_file.read(reinterpret_cast<char*>(ROM[instruction_counter]), 2)){
        ++instruction_counter;

        if (instruction_counter == ROM_SIZE) {
            memset(ROM, 0, ROM_SIZE*2);
            return false;
        }
    }

   for (int i = 0; i < instruction_counter; ++i)
       std::cout << std::hex << (int)ROM[i][0] << ' ' << (int)ROM[i][1] << std::endl;

}

void Emulator::run_program() {
    
}

void Emulator::update() {
    delay_timer = delay_timer ? delay_timer - 1 : 0;
    sound_timer = sound_timer ? sound_timer - 1 : 0;
}