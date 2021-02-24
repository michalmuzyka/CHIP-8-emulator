#include "Debugger.hpp"

Debugger::Debugger(Emulator* emulator)
    :emulator{ emulator },
    keyboard{ emulator->keyboard },
    logger{ emulator->logger },
    settings{ emulator->settings },
    window{ logger, settings }
{
    roms_path = (*settings)["rom_dir"];
    scan_roms(roms_path);
    window.open();
}

void Debugger::start_emulation() {
    auto background_color = settings->get_color("debugger_background_color");

    emulator->load_program_from_file(roms[2].string());
    emulator->window.open();
    emulator->start_emulation();
    while (emulator->window.is_open() || window.is_open()) {
        if (!emulator->window.is_open())
            emulator->stop_emulation();

        emulator->window.handle_events();
        emulator->window.display();
        emulator->update();

        window.clear(background_color);
        window.handle_events();
        draw_roms_list();
        window.display();
        update();
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

void Debugger::draw_roms_list() {
    window.draw_text("Choose game:", sf::Vector2i{0,0}, false);
    for (int i = 0; i < roms.size(); ++i)
        window.draw_text(roms[i].stem().string(), sf::Vector2i{ 1, i + 1 }, i == selected_rom);
}

void Debugger::update() {
    
}
