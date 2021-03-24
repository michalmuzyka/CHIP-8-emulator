#include <future>

#include "Emulation.hpp"

int main()
{

    auto f = std::async(std::launch::async, []() {
        Emulation emulation;
        emulation.load_program_from_file("ROMS/test_opcode.ch8");
        emulation.emulate();
    });

    f.get();

    return 0;
}
