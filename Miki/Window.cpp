#include "Window.h"
#include <tuple>
#include "CHIP8/Emulation.hpp"

bool MikiWindow::is_registered_class(HINSTANCE hInst, LPCWSTR cName) {
    WNDCLASSEX wcx;
    return GetClassInfoExW(hInst, cName, &wcx);
}

void MikiWindow::register_class(LPCWSTR cName) {
    WNDCLASSEXW wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_VREDRAW | CS_HREDRAW;
    wcx.lpfnWndProc = window_proc;
    wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wcx.lpszClassName = cName;
    ATOM result = RegisterClassExW(&wcx);
    if(!result)
        CHIP8::log(CHIP8::MESSAGE_TYPE::LOG_ERROR,
            "WinAPI window class cannot be registered, error code: " + std::to_string(GetLastError()));
}

MikiWindow::MikiWindow(HINSTANCE hIns)
    :my_hwnd{ nullptr }, hInstance(hIns) {
    
    if (!is_registered_class(hInstance, class_name.c_str()))
        register_class(class_name.c_str());

    HWND result = CreateWindowExW(0, class_name.c_str(), title.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance,
        reinterpret_cast<LPVOID>(this));

    if(!result)
        CHIP8::log(CHIP8::MESSAGE_TYPE::LOG_ERROR, 
            "WinAPI window cannot be initialized, error code: " + std::to_string(GetLastError()));
}

LRESULT MikiWindow::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MikiWindow* window = nullptr;
    if (msg == WM_NCCREATE) {
        const LPCREATESTRUCTW pcs = reinterpret_cast<LPCREATESTRUCTW>(lParam);
        window = static_cast<MikiWindow*>(pcs->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->my_hwnd = hWnd;
    } else window = reinterpret_cast<MikiWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    
    if (window) {
        const LRESULT result = window->window_proc(msg, wParam, lParam);
        if (msg == WM_NCDESTROY) {
            window->my_hwnd = nullptr;
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
        }
        return result;
    }
    
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT MikiWindow::window_proc(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg){
        case WM_CLOSE:
            DestroyWindow(my_hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        default:
            return DefWindowProcW(my_hwnd, msg, wParam, lParam);
    }
}

MikiWindow::~MikiWindow() {
    if (my_hwnd)
        DestroyWindow(my_hwnd);
}