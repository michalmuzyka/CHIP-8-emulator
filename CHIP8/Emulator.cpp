#include "Emulator.hpp"
#include <fstream>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

Emulator::Emulator(Settings* settings, Keyboard* keyboard)
    :window{ "CHIP-8 emulator", sf::Vector2i{64,32}, settings },
    settings{ settings },
    keyboard{ keyboard },
    generator{ std::chrono::high_resolution_clock::now().time_since_epoch().count() }
{
    unsigned char font[FONT_CHAR_COUNT*FONT_CHAR_SIZE] = {
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

    memcpy(RAM + FONT_LOCATION, font, FONT_CHAR_COUNT* FONT_CHAR_SIZE);
    window.link_keyboard(keyboard);
}

bool Emulator::load_program_from_file(const std::string& path) {
    PC = 0x200;
    last_instruction_addr = 0x200;

    std::ifstream program_file(path, std::ios::in | std::ios::binary);

    if (!program_file.is_open()) {
        log(MESSAGE_TYPE::ERROR, "Can't open program file");
        return false;
    }

    unsigned char buffer[2];
    while(program_file.read(reinterpret_cast<char*>(buffer), 2)){
        RAM[last_instruction_addr] = buffer[0];
        RAM[last_instruction_addr + 1] = buffer[1];

        last_instruction_addr += 2;
        if (last_instruction_addr == RAM_SIZE) {
            last_instruction_addr = 0x200;
            log(MESSAGE_TYPE::ERROR, "File is too big to load");
            return false;
        }
    }
 
    return true;
}

void Emulator::update() {
    if (emulate) {
        delay_timer = delay_timer ? delay_timer - 1 : 0;
        sound_timer = sound_timer ? sound_timer - 1 : 0;

        if (sound_timer) window.play_buzzer();

        if (PC < last_instruction_addr)
            execute_current_line();
    }
}

void Emulator::start_emulation() {
    emulate = true;
}

void Emulator::stop_emulation() {
    emulate = false;
}


void Emulator::execute_current_line() {
    unsigned char hex_chars[4];
    hex_chars[0] = (RAM[PC] & 0b11110000) >> 4;
    hex_chars[1] = RAM[PC] & 0b1111;
    hex_chars[2] = (RAM[PC + 1] & 0b11110000) >> 4;
    hex_chars[3] = RAM[PC + 1] & 0b1111;

    switch (hex_chars[0]) {
        case 0x0: opcodes0(hex_chars); break;
        case 0x1: opcodes1(hex_chars); break;
        case 0x2: opcodes2(hex_chars); break;
        case 0x3: opcodes3(hex_chars); break;
        case 0x4: opcodes4(hex_chars); break;
        case 0x5: opcodes5(hex_chars); break;
        case 0x6: opcodes6(hex_chars); break;
        case 0x7: opcodes7(hex_chars); break;
        case 0x8: opcodes8(hex_chars); break;
        case 0x9: opcodes9(hex_chars); break;
        case 0xA: opcodesA(hex_chars); break;
        case 0xB: opcodesB(hex_chars); break;
        case 0xC: opcodesC(hex_chars); break;
        case 0xD: opcodesD(hex_chars); break;
        case 0xE: opcodesE(hex_chars); break;
        case 0xF: opcodesF(hex_chars); break;
        default: unknown_opcode(hex_chars);
    }
    PC_should_be_increment ? PC += 2 : PC_should_be_increment = true;
}

void Emulator::opcodes0(const unsigned char hex_chars[4]) {
    if (hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0x0) //00E0, clear display
        window.clear();
    else if (hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0xE) { //00EE, return from subroutine
        if (stack.empty())
            log(MESSAGE_TYPE::ERROR, "CANNOT RETURN FROM SUBROUTINE");
        PC = stack.top();
        stack.pop();
    } else unknown_opcode(hex_chars);
}

void Emulator::opcodes1(const unsigned char hex_chars[4]) {
    //1NNN, jump to address
    PC = get_address_from_binary(hex_chars);
    PC_should_be_increment = false;
}

void Emulator::opcodes2(const unsigned char hex_chars[4]) {
    // 2NNN, run subroutine
    stack.push(PC);
    PC = get_address_from_binary(hex_chars);
    PC_should_be_increment = false;
}

void Emulator::opcodes3(const unsigned char hex_chars[4]) {
    // 3XNN, skip if Vx == NN  
    if (V[hex_chars[1]] == get_constant_from_binary(hex_chars))
        PC += 2;
}

void Emulator::opcodes4(const unsigned char hex_chars[4]) {
    // 4XNN, skip if Vx != NN 
    if (V[hex_chars[1]] != get_constant_from_binary(hex_chars))
        PC += 2;
}

void Emulator::opcodes5(const unsigned char hex_chars[4]) {
    // 5XY0, skip if Vx == Vy
    if (V[hex_chars[1]] == V[hex_chars[2]])
        PC += 2;
}

void Emulator::opcodes6(const unsigned char hex_chars[4]) {
    // 6XNN, Vx = NN
    V[hex_chars[1]] = get_constant_from_binary(hex_chars);
}

void Emulator::opcodes7(const unsigned char hex_chars[4]) {
    // 7XNN, Vx += NN
    V[hex_chars[1]] += get_constant_from_binary(hex_chars);
}

void Emulator::opcodes8(const unsigned char hex_chars[4]) {
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
        V[0xF] = static_cast<int>(V[hex_chars[1]]) > static_cast<int>(V[hex_chars[2]]) ? 1 : 0;
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
        V[0xF] = V[hex_chars[1]] & 0b10000000;
        V[hex_chars[1]] <<= 1;
        break;
    default: unknown_opcode(hex_chars);
    }
}

void Emulator::opcodes9(const unsigned char hex_chars[4]) {
    if (hex_chars[3] == 0x0) {  // 9XY0, skip if Vx != Vy
        if (V[hex_chars[1]] != V[hex_chars[2]])
            PC += 2;
    } else unknown_opcode(hex_chars);
}

void Emulator::opcodesA(const unsigned char hex_chars[4]) {
    // ANNN, set I to address
    I = get_address_from_binary(hex_chars);
}

void Emulator::opcodesB(const unsigned char hex_chars[4]) {
    // BNNN, jump to address + V0
    PC = static_cast<int>(V[0]) + get_address_from_binary(hex_chars);
    PC_should_be_increment = false;
}

void Emulator::opcodesC(const unsigned char hex_chars[4]) {
    // CNNN, Vx = rand()%NN
    const std::uniform_int_distribution<int> uniform_dist(0, static_cast<int>(get_constant_from_binary(hex_chars)));
    V[hex_chars[1]] = uniform_dist(generator);
}

void Emulator::opcodesD(const unsigned char hex_chars[4]) {
    // DXYN, display
    bool pixel_changed = false;
    for (unsigned char i = 0x0; i < hex_chars[3]; ++i)
        pixel_changed |= window.draw_pixels_row(sf::Vector2i{ V[hex_chars[1]], V[hex_chars[2]]+i }, RAM[I + i]);

    V[0xF] = pixel_changed ? 1 : 0;
}

void Emulator::opcodesE(const unsigned char hex_chars[4]) {
    if (hex_chars[2] == 0x9 && hex_chars[3] == 0xE) { // EX9E, skip next if key from Vx is pressed
        if (keyboard->CHIP8_key_is_pressed(V[hex_chars[1]]))
            PC += 2;
    }
    else if (hex_chars[2] == 0xA && hex_chars[3] == 0x1) { // EXA1, skip next if key from Vx isnt pressed
        if (!keyboard->CHIP8_key_is_pressed(V[hex_chars[1]]))
            PC += 2;
    }
    else unknown_opcode(hex_chars);
}

void Emulator::opcodesF(const unsigned char hex_chars[4]) {
    if (hex_chars[2] == 0x0 && hex_chars[3] == 0x7) // FX07, set Vx to delay timer
        V[hex_chars[1]] = delay_timer;
    else if (hex_chars[2] == 0x0 && hex_chars[3] == 0xA) {  // FX0A, block until key pressed
        PC_should_be_increment = keyboard->is_any_pressed();
    }
    else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x5) // FX15, set delay timer to Vx
        delay_timer = V[hex_chars[1]];
    else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x8) // FX15, set sound timer to Vx
        sound_timer = V[hex_chars[1]];
    else if (hex_chars[2] == 0x1 && hex_chars[3] == 0xE) // FX1E, I += Vx
        I += V[hex_chars[1]];
    else if (hex_chars[2] == 0x2 && hex_chars[3] == 0x9) // FX29, I = location of sprite for digit stored in Vx
        I = FONT_LOCATION + V[hex_chars[1]] * FONT_CHAR_SIZE;
    else if (hex_chars[2] == 0x3 && hex_chars[3] == 0x3) { // FX33 
        RAM[I] = V[hex_chars[1]] / 100;
        RAM[I + 1] = (V[hex_chars[1]] / 10) % 10;
        RAM[I + 2] = (V[hex_chars[1]]) % 10;
    }
    else if (hex_chars[2] == 0x5 && hex_chars[3] == 0x5) { // FX55
        for (unsigned i = 0x0; i <= hex_chars[1]; ++i)
            RAM[I + i] = V[i];
        I += hex_chars[1];
    }
    else if (hex_chars[2] == 0x6 && hex_chars[3] == 0x5) { // FX65
        for (unsigned i = 0x0; i <= hex_chars[1]; ++i)
            V[i] = RAM[I + i];
        I += hex_chars[1];

    }
    else unknown_opcode(hex_chars);
}

int Emulator::get_address_from_binary(const unsigned char hex_chars[4]) {
    return (hex_chars[1] << 8) + (hex_chars[2] << 4) + hex_chars[3];
}

unsigned char Emulator::get_constant_from_binary(const unsigned char hex_chars[4]) {
    return (hex_chars[2] << 4) + hex_chars[3];
}

void Emulator::unknown_opcode(const unsigned char hex_chars[4]) const {
    std::stringstream ss;
    ss << std::hex << (int)(hex_chars[0]) << (int)(hex_chars[1]) << (int)(hex_chars[2]) << (int)(hex_chars[3]);
    log(MESSAGE_TYPE::INFO, "Unknown opcode: " + ss.str());
}
