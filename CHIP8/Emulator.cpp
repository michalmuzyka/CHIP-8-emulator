#include "Emulator.hpp"
#include <fstream>
#include <cstring>
#include <iostream>

Emulator::Emulator(Logger* logger, GameWindow* window, Keyboard* keyboard)
    :logger{logger}, window{window}, keyboard{keyboard}
{
    unsigned char font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

    memcpy(RAM + 0x50, font, 80);

    load_program_from_file("test_opcode.ch8");
}

bool Emulator::load_program_from_file(const std::string& path) {
    std::ifstream program_file(path, std::ios::in | std::ios::binary);

    if (!program_file.is_open()) {
        if(logger) 
            logger->log(MESSAGE_TYPE::ERROR, "Can't open program file");
        return false;
    }

    unsigned char buffer[2];
    while(program_file.read(reinterpret_cast<char*>(buffer), 2)){
        RAM[last_instruction_addr] = buffer[0];
        RAM[last_instruction_addr + 1] = buffer[1];

        last_instruction_addr += 2;
        if (last_instruction_addr == RAM_SIZE) {
            last_instruction_addr = 0x200;
            if (logger) 
                logger->log(MESSAGE_TYPE::ERROR, "File is too big to load");
            return false;
        }
    }
    
    return true;
}

void Emulator::update() {
    delay_timer = delay_timer ? delay_timer - 1 : 0;
    sound_timer = sound_timer ? sound_timer - 1 : 0;

    if(PC < last_instruction_addr)
        execute_current_line();
}

void Emulator::execute_current_line() {
    unsigned char hex_chars[4];
    hex_chars[0] = (RAM[PC] & 0b11110000) >> 4;
    hex_chars[1] = RAM[PC] & 0b1111;
    hex_chars[2] = (RAM[PC + 1] & 0b11110000) >> 4;
    hex_chars[3] = RAM[PC + 1] & 0b1111;

    switch (hex_chars[0]) {
        case 0x0:
            if (hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0x0) //00E0, display clear
                window->clear();
            else if (hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0xE){ //00EE, return from subroutine
                if (stack.empty()) logger->log(MESSAGE_TYPE::ERROR, "CANNOT RETURN FROM SUBROUTINE");
                PC = stack.top();
                stack.pop();
            }
            break;
        case 0x1: // 1NNN, jump to address
            PC = get_address_from_binary(hex_chars);
            PC_should_be_increment = false;
            break;
        case 0x2: // 2NNN, run subroutine
            stack.push(PC);
            PC = get_address_from_binary(hex_chars);
            PC_should_be_increment = false;
            break;
        case 0x3: // 3XNN, skip if Vx == NN  
            if (V[hex_chars[1]] == get_constant_from_binary(hex_chars))
                PC += 2;
            break;
        case 0x4: // 4XNN, skip if Vx != NN 
            if (V[hex_chars[1]] != get_constant_from_binary(hex_chars))
                PC += 2;
            break;
        case 0x5: // 5XY0, skip if Vx == Vy
            if (V[hex_chars[1]] == V[hex_chars[2]])
                PC += 2;
            break;
        case 0x6: // 6XNN, set Vx = NN
            V[hex_chars[2]] = get_constant_from_binary(hex_chars);
            break;
        case 0x7: // 7XNN, Vx += NN
            V[hex_chars[2]] += get_constant_from_binary(hex_chars);
            break;
        case 0x8: 
            switch (hex_chars[3]) {
                case 0x0: // 8XY0, Vx = Vy
                    V[hex_chars[1]] = V[hex_chars[2]];
                    break;
                case 0x1:  // 8XY1, Vx = Vx OR Vy
                    V[hex_chars[1]] |= V[hex_chars[2]];
                    break;
                case 0x2:  // 8XY2, Vx = Vx AND Vy 
                    V[hex_chars[1]] &= V[hex_chars[2]];
                    break;
                case 0x3:  // 8XY3, Vx = Vx XOR Vy
                    V[hex_chars[1]] ^= V[hex_chars[2]];
                    break;
                case 0x4:  // 8XY4, Vx += Vy
                    V[0xF] = static_cast<int>(V[hex_chars[1]]) + static_cast<int>(V[hex_chars[2]]) > UCHAR_MAX ? 1 : 0;
                    V[hex_chars[1]] += V[hex_chars[2]];
                    break;
                case 0x5:  // 8XY5, Vx -= Vy
                    V[0xF] = static_cast<int>(V[hex_chars[1]]) - static_cast<int>(V[hex_chars[2]]) < 0 ? 1 : 0;
                    V[hex_chars[1]] -= V[hex_chars[2]];
                    break;
                case 0x6:  // 8XY6, Vx >>= 1
                    V[0xF] = V[hex_chars[1]] & 0b1;
                    V[hex_chars[1]] >>= 1;
                    break;
                case 0x7:  // 8XY7, Vx = Vy - Vx
                    V[0xF] = static_cast<int>(V[hex_chars[2]]) - static_cast<int>(V[hex_chars[1]]) < 0 ? 1 : 0;
                    V[hex_chars[1]] = V[hex_chars[2]] - V[hex_chars[1]];
                    break;
                case 0xE:  // 8XYE, Vx <<= 1
                    V[0xF] = V[hex_chars[2]] & 0b10000000;
                    V[hex_chars[2]] <<= 1;
                    break;
            }
            break;
        case 0x9: 
            if (hex_chars[3] == 0x0)  // 9XY0, skip if Vx != Vy
                if (V[hex_chars[1]] != V[hex_chars[2]])
                    PC += 2;
            break;
        case 0xA: // ANNN, set I to address
            I = get_address_from_binary(hex_chars);
            break;
        case 0xB: // BNNN, jump to adress + V0
            PC = static_cast<int>(V[0]) + get_address_from_binary(hex_chars);
            PC_should_be_increment = false;
            break;
        case 0xC: // CNNN, Vx = rand()%NN
            const std::uniform_int_distribution<unsigned char> uniform_dist(0, get_constant_from_binary(hex_chars));
            V[hex_chars[1]] = uniform_dist(generator);
            break;
        case 0xD: // DXYN, display
            
            break;
        case 0xE: 
            if (hex_chars[2] == 0x9 && hex_chars[3] == 0xE) // EX9E, skips next if key from Vx is pressed
                
            else if (hex_chars[2] == 0xA && hex_chars[3] == 0x1) // EXA1, skips next if key from Vx isnt pressed
                
            break;
        case 0xF:
            if (hex_chars[2] == 0x0 && hex_chars[3] == 0x7) // FX07, sets delay timer to Vx

            else if (hex_chars[2] == 0x0 && hex_chars[3] == 0xA)  // FX0A, block until key pressed

            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x5) // FX15, sets delay timer to Vx
                delay_timer = V[hex_chars[1]];
            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x8) // FX15, sets sound timer to Vx
                sound_timer = V[hex_chars[1]];
            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0xE) // FX1E, I += Vx
                I += V[hex_chars[1]];
            else if (hex_chars[2] == 0x2 && hex_chars[3] == 0x9) // FX29
               
            else if (hex_chars[2] == 0x3 && hex_chars[3] == 0x3) // FX33 
                
            else if (hex_chars[2] == 0x5 && hex_chars[3] == 0x5) // FX55
                
            else if (hex_chars[2] == 0x6 && hex_chars[3] == 0x5) // FX65
                
            break;
    }
    PC_should_be_increment ? PC += 2 : PC_should_be_increment = true;
}

int Emulator::get_address_from_binary(const unsigned char hex_chars[4]) {
    return (hex_chars[1] << 8) + (hex_chars[2] << 4) + hex_chars[3];
}

unsigned char Emulator::get_constant_from_binary(const unsigned char hex_chars[4]) {
    return (hex_chars[2] << 4) + hex_chars[3];
}