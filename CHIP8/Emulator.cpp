#include "Emulator.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

Emulator::Emulator(Logger* logger, Window* window, Keyboard* keyboard)
{
    devices.window = window;
    devices.keyboard = keyboard;

    load_program_from_file("test_opcode.ch8");
}

bool Emulator::load_program_from_file(const std::string& path) {
    std::ifstream program_file(path, std::ios::in | std::ios::binary);

    if (!program_file.is_open()) {
        if(devices.logger) 
            devices.logger->log(MESSAGE_TYPE::ERROR, "Can't open program file");
        return false;
    }

    instruction_count = 0;
    unsigned char buffer[2];
    while(program_file.read(reinterpret_cast<char*>(buffer), 2)){
        ++instruction_count;
        program[instruction_count].create_from_binary(buffer);

        if (instruction_count == ROM_SIZE) {
            instruction_count = 0;
            if (devices.logger) 
                devices.logger->log(MESSAGE_TYPE::ERROR, "File is too big to load to ROM");
            return false;
        }
    }
    
    return true;
}

void Emulator::execute_current_line() {
    if (devices.PC < instruction_count) {
        program[devices.PC].execute(&devices);
        devices.PC_should_be_increment ? devices.PC++ : devices.PC_should_be_increment = true;
    }
}

void Emulator::update() {
    devices.delay_timer = devices.delay_timer ? devices.delay_timer - 1 : 0;
    devices.sound_timer = devices.sound_timer ? devices.sound_timer - 1 : 0;
    execute_current_line();
}