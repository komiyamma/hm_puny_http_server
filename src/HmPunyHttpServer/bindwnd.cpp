#include <windows.h>

static HWND bindWnd = NULL;

void BindWindow(HWND hwnd) {
	bindWnd = hwnd;
}

bool IsBindWindowClosed() {
	if (bindWnd && !IsWindow(bindWnd)) {
		return true;
	}
	return false;
}