#include "Launcher.hpp"
#include "CHIP8/Emulation.hpp"

void Launcher::start_emulation(wchar_t* path) {
    if (emulation_running)
        return;

    emulation_running = true;
    emulation = std::thread([this, path]() {
        CHIP8::Emulation emulation;
        if (emulation.load_program_from_file(path))
            emulation.emulate(std::ref(emulation_running));
        });
}

void Launcher::stop_emulation() {
    if (emulation_running) {
        emulation_running = false;
        emulation.join();
    }
}


