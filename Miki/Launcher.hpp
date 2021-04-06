#pragma once
#include <thread>

class Launcher
{
public:
    void stop_emulation();
    void start_emulation(wchar_t*);

private:
    std::thread emulation;
    std::atomic_bool emulation_running{ false };
};
