#pragma once
#include <string>
#define NOMINMAX
#include <vector>
#include <Windows.h>
#include "TableLayout.hpp"

struct ControlData {
    HWND hwnd{nullptr};
    LPCWSTR class_name;
    LPCWSTR name;
    int pos_x;
    int pos_y;
    int width;
    int height;
    HMENU id;
    UINT styles;
    ControlData(LPCWSTR class_name, LPCWSTR name, int pos_x, int pos_y, int width, int height, HMENU id, UINT styles)
        :class_name{class_name}, name{name}, pos_x{pos_x}, pos_y{pos_y}, width{width}, height{height}, id{id}, styles{styles}{}
};

class MikiWindow
{
public:
    static LRESULT window_proc(HWND, UINT, WPARAM, LPARAM); //window callback
    LRESULT window_proc(UINT, WPARAM, LPARAM);
    MikiWindow(HINSTANCE);
    MikiWindow(const MikiWindow& window) = delete;
    MikiWindow& operator=(const MikiWindow& window) = delete;
    HWND operator()() const { return my_hwnd; }
    ~MikiWindow();
private:
    bool is_registered_class(LPCWSTR) const;
    void register_class(LPCWSTR) const;
    void set_appearance() const;
    void create_buttons();

    HWND my_hwnd, textbox_hwnd;
    DWORD main_window_styles = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE;
    HINSTANCE hInstance;
    std::wstring class_name = L"MyWindowClass";
    std::wstring title = L"Miki";
    const static int ID_RUN = 101; 
    const static int ID_STOP = 102;
    const static int ID_BROWSE = 103;
    const static int ID_TEXTBOX = 104;

    const TableLayout table{15, 4, 20, 25};

    const int window_width = table.get_table_width();
    const int window_height = table.get_table_height();

    std::vector<ControlData> controls {
        {L"Static", L"Enter ROM file:", table.col(1), table.row(0.6), table.width(0, 65), 23, nullptr, WS_VISIBLE | WS_CHILD | SS_LEFT | SS_SIMPLE},
        {L"Button", L"Stop", table.col(1,0), table.row(-1.4), table.width(-1.2, 50), table.height(1), reinterpret_cast<HMENU>(ID_STOP), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON},
        {L"Button", L"Run", table.col(0.2, 50), table.row(-1.4), table.width(-1.2, 50), table.height(1), reinterpret_cast<HMENU>(ID_RUN), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON},
        {L"Button", L"Browse", table.col(1, 70), table.row(1.3), table.width(-2, 30), table.height(1), reinterpret_cast<HMENU>(ID_BROWSE), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON},
        {L"Edit", L"", table.col(1, 0), table.row(1.3) + 1, table.width(0, 67), 23, reinterpret_cast<HMENU>(ID_TEXTBOX), WS_VISIBLE | WS_CHILD | WS_BORDER}
    };
};