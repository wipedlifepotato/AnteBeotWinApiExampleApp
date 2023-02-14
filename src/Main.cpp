#include"MainWindow.h"
#ifndef UNICODE
#define UNICODE
#endif
#include <gdiplus.h>

using namespace Gdiplus;
int gCmdShow;
HINSTANCE g_hInst;
extern "C" {
    // HINSTANCE, HINSTANCE, LPSTR, int
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
    {
        g_hInst = hInstance;
        gCmdShow = nCmdShow;
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR           gdiplusToken;


        // Initialize GDI+.
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


        MainWindow win;

        assert(win.Create(L"Let Wall go out", WS_OVERLAPPEDWINDOW));

        const HWND & hwnd = win.getMHWND();
        HMENU hMainMenu;
        hMainMenu = CreateMenu();
        HMENU hPopMenuFile = CreatePopupMenu();

        AppendMenu(hMainMenu, MF_STRING | MF_POPUP, reinterpret_cast<unsigned long long>((unsigned long long)hPopMenuFile), L"Программа");
        //AppendMenu(hMainMenu, MF_STRING, MENU_EXIT_ITEM, L"Выход");

        AppendMenu(hPopMenuFile, MF_STRING, MENU_EXIT_ITEM, L"Выход");

        SetMenu(hwnd, hMainMenu);
        SetMenu(hwnd, hPopMenuFile);

        ShowWindow(win.Window(), nCmdShow);

        // Run the message loop.

        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }

        return 0;
    }
}
