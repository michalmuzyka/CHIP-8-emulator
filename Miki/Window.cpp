#include "Window.h"


bool Window::is_registered_class(HINSTANCE hInst, LPCWSTR cName) {


}


void Window::register_class(HINSTANCE hInst, LPCWSTR cName) {
	WNDCLASSEXW wcx{};
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_VREDRAW | CS_HREDRAW;
	wcx.lpfnWndProc = window_proc;
	wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcx.lpszClassName = cName;
	RegisterClassExW(&wcx);
}


