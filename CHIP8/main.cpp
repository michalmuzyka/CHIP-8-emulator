#include <future>
#include <chrono>
#include "Emulation.hpp"

int main()
{
    /////////////// 
    /// Example
    ///////////////

    std::atomic_bool stop(false);
    auto f = std::async(std::launch::async, [&stop]() {
        CHIP8::Emulation emulation;
        emulation.load_program_from_file("ROMS/test_opcode.ch8");
        emulation.emulate(std::ref(stop));
    });

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
    stop = true;
    f.get();

    return 0;
}
