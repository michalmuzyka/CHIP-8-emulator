#include "Debugger.hpp"
#include <iostream>

Debugger::Debugger(Emulator* emulator)
    :emulator{emulator}, keyboard{emulator->keyboard}, logger{emulator->logger}, settings{emulator->settings}
{
    roms_path = (*settings)["rom_dir"];
    scan_roms(roms_path);
}

void Debugger::start_emulation() {
    emulator->load_program_from_file(roms[2].string());
    emulator->window.open();
    while (emulator->window.is_open()) {
        emulator->window.handle_events();
        emulator->window.display();

        emulator->update();
    }
}

void Debugger::scan_roms(const std::string &directory_path) {
    namespace fs = std::filesystem;
    if (!fs::exists(directory_path))
        logger->log(MESSAGE_TYPE::ERROR, directory_path + " doesn't exist");
    if (!fs::is_directory(directory_path))
        logger->log(MESSAGE_TYPE::ERROR, directory_path + " is not a directory");

    for(auto& path : fs::recursive_directory_iterator(directory_path)){
        if(path.is_regular_file())
            roms.push_back(path.path());
    }
}