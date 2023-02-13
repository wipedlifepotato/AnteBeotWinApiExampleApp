#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"BaseWindow.h"
#define ID_LOGIN_ENTRY 1
#define ID_PASSWORD_ENTRY 2
#define MENU_EXIT_ITEM 1000



class MainWindow : public BaseWindow<MainWindow>
{
private:
    VOID OnCreate(void);

public:
    PCWSTR  ClassName() const
    {
        return L"MainWindowClass";
    }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // MAINWINDOW_H
