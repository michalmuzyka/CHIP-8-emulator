#include "Debugger.hpp"

Debugger::Debugger(Emulator* emulator)
    :emulator{emulator}, keyboard{emulator->keyboard}, logger{emulator->logger}
{
    
}

void Debugger::start_emulation() {
    while (emulator->window.is_open()) {
        emulator->window.handle_events();
        emulator->window.display();

        emulator->update();
    }
}
