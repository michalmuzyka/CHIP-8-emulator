#include "Opcode.hpp"

int Opcode::get_PC_address_from_binary(const unsigned char hex_chars[4]) {
    return (hex_chars[1] << 8) + (hex_chars[2] << 4) + hex_chars[3] - 0x200;
}

void Opcode::create_from_binary(const unsigned char opcode[2]) {
    unsigned char hex_chars[4];
    hex_chars[0] = (opcode[0] & 0b11110000) >> 4;
    hex_chars[1] = opcode[0] & 0b1111;
    hex_chars[2] = (opcode[1] & 0b11110000) >> 4;
    hex_chars[3] = opcode[1] & 0b1111;

    switch (hex_chars[0]) {
        case 0x0:
            if(hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0x0) //00E0, display clear
                execute = [](Devices* dev) { dev->window->clear(); };
            else if (hex_chars[1] == 0x0 && hex_chars[2] == 0xE && hex_chars[3] == 0xE) //00EE, return from subroutine
                execute = [](Devices* dev){
                    if (dev->stack.empty()) dev->logger->log(MESSAGE_TYPE::ERROR, "CANNOT RETURN FROM SUBROUTINE");
                    dev->PC = dev->stack.top();
                    dev->stack.pop();
                    dev->PC_should_be_increment = false;
                };
            break;
        case 0x1: 
            execute = [hex_chars](Devices* dev){
                const int address = get_PC_address_from_binary(hex_chars);
                dev->PC = address;
                dev->PC_should_be_increment = false;
            };
            break;
        case 0x2: 
            execute = [](Devices* dev) {};
            break;
        case 0x3:
            execute = [](Devices* dev) {};
            break;
        case 0x4: 
            execute = [](Devices* dev) {};
            break;
        case 0x5: 
            execute = [](Devices* dev) {};
            break;
        case 0x6:
            execute = [](Devices* dev) {};
            break;
        case 0x7: 
            execute = [](Devices* dev) {};
            break;
        case 0x8: 
            switch (hex_chars[3]) {
                case 0x0: execute = [](Devices* dev) {}; break;
                case 0x1: execute = [](Devices* dev) {}; break;
                case 0x2: execute = [](Devices* dev) {}; break;
                case 0x3: execute = [](Devices* dev) {}; break;
                case 0x4: execute = [](Devices* dev) {}; break;
                case 0x5: execute = [](Devices* dev) {}; break;
                case 0x6: execute = [](Devices* dev) {}; break;
                case 0x7: execute = [](Devices* dev) {}; break;
                case 0xE: execute = [](Devices* dev) {}; break;
            }
            break;
        case 0x9: 
            if (hex_chars[3] == 0x0)
                execute = [](Devices* dev) {};
            break;
        case 0xA: 
            execute = [](Devices* dev) {};
            break;
        case 0xB: 
            execute = [](Devices* dev) {};
            break;
        case 0xC: 
            execute = [](Devices* dev) {};
            break;
        case 0xD: 
            execute = [](Devices* dev) {};
            break;
        case 0xE: 
            if (hex_chars[2] == 0x9 && hex_chars[3] == 0xE)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0xA && hex_chars[3] == 0x1)
                execute = [](Devices* dev) {};
            break;
        case 0xF: 
            if (hex_chars[2] == 0x0 && hex_chars[3] == 0x7)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x0 && hex_chars[3] == 0xA)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x5)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0x8)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x1 && hex_chars[3] == 0xE)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x2 && hex_chars[3] == 0x9)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x3 && hex_chars[3] == 0x3)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x5 && hex_chars[3] == 0x5)
                execute = [](Devices* dev) {};
            else if (hex_chars[2] == 0x6 && hex_chars[3] == 0x5)
                execute = [](Devices* dev) {};
            break;
    }
}