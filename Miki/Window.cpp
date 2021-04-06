#include "Window.hpp"
#include <thread>
#include "CHIP8/Emulation.hpp"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") //enabling win10 visual styles

bool MikiWindow::is_registered_class(LPCWSTR cName) const{
    WNDCLASSEX wcx;
    return GetClassInfoExW(hInstance, cName, &wcx);
}

void MikiWindow::register_class(LPCWSTR cName) const{
    WNDCLASSEXW wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_VREDRAW | CS_HREDRAW;
    wcx.lpfnWndProc = window_proc;
    wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wcx.lpszClassName = cName;
    if(!RegisterClassExW(&wcx))
        CHIP8::log(CHIP8::LOG_ERROR,"WinApi window class, error code: " + std::to_string(GetLastError()));
}

void MikiWindow::set_appearance() const{
    RECT rect = {0, 0 , window_width, window_height};
    AdjustWindowRectEx(&rect, main_window_styles, false, 0);
    const int non_client_width = rect.right - rect.left;
    const int non_client_height = rect.bottom - rect.top;
    const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - non_client_width;
    const int y = (GetSystemMetrics(SM_CYSCREEN) - non_client_height) / 2;
    SetWindowPos(my_hwnd, nullptr, x, y, non_client_width, non_client_height, SWP_NOZORDER);
}

void MikiWindow::create_buttons() {
    for (auto& data : controls) {
        data.hwnd = CreateWindowExW(0, data.class_name, data.name, data.styles, data.pos_x, data.pos_y, data.width, data.height, my_hwnd, data.id, hInstance, nullptr);

        if (!data.hwnd) 
            CHIP8::log(CHIP8::LOG_ERROR, "WinApi: button init, error code: " + std::to_string(GetLastError()));

        SendMessageW(data.hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
        if(data.id == reinterpret_cast<HMENU>(ID_TEXTBOX))
            textbox_hwnd = data.hwnd;
    }
}

MikiWindow::MikiWindow(HINSTANCE hIns)
    :my_hwnd{ nullptr }, hInstance(hIns)
{
    if (!is_registered_class(class_name.c_str()))
        register_class(class_name.c_str());

    if (!CreateWindowExW(0, class_name.c_str(), title.c_str(), main_window_styles, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, reinterpret_cast<LPVOID>(this))) 
        CHIP8::log(CHIP8::LOG_ERROR, "WinApi: window init, error code: " + std::to_string(GetLastError()));
}

LRESULT MikiWindow::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MikiWindow* window { nullptr };
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
        case WM_CREATE:
            set_appearance();
            create_buttons();
            break;
        case WM_COMMAND: 
            handle_gui_behaviour(LOWORD(wParam));
            break;
        case WM_CTLCOLORSTATIC:
            return reinterpret_cast<LRESULT>(GetStockObject(HOLLOW_BRUSH));
        case WM_CLOSE:
            launcher.stop_emulation();
            DestroyWindow(my_hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        default: break;
    }
    return DefWindowProcW(my_hwnd, msg, wParam, lParam);
}

void MikiWindow::handle_gui_behaviour(DWORD id) {
    switch (id) {
        case ID_RUN: {
                GetWindowTextW(textbox_hwnd, path, MAX_PATH);
                launcher.start_emulation(path); 
            } break;
        case ID_STOP: launcher.stop_emulation(); break;
        case ID_TEXTBOX: break;
        case ID_BROWSE: browse_file(); break;
        default:
            CHIP8::log(CHIP8::LOG_ERROR, "WinApi: wrong control ID");
    }
}

void MikiWindow::browse_file() {
    OPENFILENAMEW filename;
    memset(&filename, 0, sizeof(OPENFILENAMEW));
    filename.hInstance = hInstance;
    filename.lStructSize = sizeof(OPENFILENAMEW);
    filename.lpstrDefExt = L"\0";
    filename.hwndOwner = my_hwnd;
    filename.lpstrFile = path;
    filename.nMaxFile = MAX_PATH;
    filename.lpstrTitle = L"Choose ROM file:";
    filename.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&filename))
        SetWindowTextW(textbox_hwnd, path);
}


MikiWindow::~MikiWindow() {
    launcher.stop_emulation();
    if (my_hwnd) DestroyWindow(my_hwnd);
    for (auto& data : controls)
        if(data.hwnd) DestroyWindow(data.hwnd);
}