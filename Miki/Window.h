#pragma once
#include <string>
#include <utility>
#include <Windows.h>

class Window
{
private:
	static bool is_registered_class(HINSTANCE, LPCWSTR);
	static void register_class(HINSTANCE, LPCWSTR);
	HWND my_hwnd;
public:
	static LRESULT window_proc(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT window_proc(UINT, WPARAM, LPARAM);
	Window() :my_hwnd{ nullptr } {  }
	HWND operator()() { return my_hwnd; }
};