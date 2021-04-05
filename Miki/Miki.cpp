#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "Window.hpp"
#include "CHIP8/Emulation.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MikiWindow miki(hInstance);
    MSG msg;
    BOOL gmResult;
    while ((gmResult = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (gmResult == -1)
            CHIP8::log(CHIP8::MESSAGE_TYPE::LOG_ERROR, "Winapi error code: " + std::to_string(GetLastError()));
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return msg.wParam;
}