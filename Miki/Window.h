#pragma once
#include <string>
#include <utility>
#define NOMINMAX
#include <Windows.h>

class MikiWindow
{
private:
	static bool is_registered_class(HINSTANCE, LPCWSTR);
	static void register_class(LPCWSTR);
	HWND my_hwnd;
	HINSTANCE hInstance;
	std::wstring class_name = L"MyWindowClass";
    std::wstring title = L"Miki";

public:
	static LRESULT window_proc(HWND, UINT, WPARAM, LPARAM); //window callback
	LRESULT window_proc(UINT, WPARAM, LPARAM);
	MikiWindow(HINSTANCE);
	MikiWindow(const MikiWindow& window) = delete;
	MikiWindow& operator=(const MikiWindow& window) = delete;
	HWND operator()() const { return my_hwnd; }
	~MikiWindow();
};