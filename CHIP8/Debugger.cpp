#include "Debugger.hpp"


Debugger::Debugger(Emulator* emulator)
    :emulator{emulator}, keyboard{emulator->keyboard}, logger{emulator->logger}
{
    scan_roms(roms_path);
}

void Debugger::start_emulation() {
    while (emulator->window.is_open()) {
        emulator->window.handle_events();
        emulator->window.display();

        emulator->update();
    }
}

void Debugger::scan_roms(std::string directory_path) {
    for(auto &path : std::filesystem::recursive_directory_iterator(directory_path)){
        if(path.is_regular_file())
            roms.push_back(path.path());
    }
}