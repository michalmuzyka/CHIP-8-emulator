#include <thread>
#include "Emulation.hpp"

void emulation(std::wstring path){

    std::atomic_bool should_run;

    //start emulation
    std::thread thread = std::thread([path, &should_run]() {
        CHIP8::Emulation emulation;
        if (emulation.load_program_from_file(path))
            emulation.emulate(std::ref(should_run));
    });

    //stop
    should_run = false;
    thread.join();
}