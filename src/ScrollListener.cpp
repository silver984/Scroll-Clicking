#include <Windows.h>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

WNDPROC g_originalWndProc = nullptr;
bool g_hooked = false;
bool g_hookAttempted = false;
bool g_mouseScrolled = false;

LRESULT CALLBACK customWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_MOUSEWHEEL)
        g_mouseScrolled = true;
    return CallWindowProcW(g_originalWndProc, hwnd, uMsg, wParam, lParam);
}

void hookWndProc() {
    if (g_hooked || g_hookAttempted)
        return;

    g_hookAttempted = true;

    HWND hwnd = GetForegroundWindow();
    if (!hwnd) {
        log::error("Failed to get game window handle.");
        return;
    }

    // install the hook only once
    g_originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(customWndProc)));
    if (g_originalWndProc) {
        g_hooked = true;
        log::info("Successfully hooked window procedure.");
    }
    else {
        log::error("Failed to hook window procedure.");
    }
}
