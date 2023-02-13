#include"MainWindow.h"
#include"Util.h"
#include"AuthWindow.h"
//#include<thread>
static AuthWindow authWin;
VOID MainWindow::OnCreate(void)
{
        if (!updateSession())
        {
            CreateWindow(L"button",L"АВТОРИЗАЦИЯ",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     0,0,150,20,m_hwnd,(HMENU) ID_LOGIN_ENTRY,NULL,NULL);
            CreateWindow(L"button",L"РЕГИСТРАЦИЯ",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     0,50,150,20,m_hwnd,(HMENU) ID_PASSWORD_ENTRY,NULL,NULL);
            assert(authWin.Create(L"Authentiphication", WS_OVERLAPPEDWINDOW));
        }
        else
        {
            MessageBox(m_hwnd, L"Сессия была восстановлена", L"About session", MB_OK|MB_ICONQUESTION);
            CreateWindow(L"button",L"DUMMY",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                 0,0,150,20,m_hwnd,(HMENU) ID_LOGIN_ENTRY,NULL,NULL);
        }
}

static VOID OnPaint(HDC hdc, PAINTSTRUCT & ps)
{
    // All painting occurs here, between BeginPaint and EndPaint.
    ps.fErase = TRUE;
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

    //Graphics graphics(hdc);
}




LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;

    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        exit(0);
        return 0;
    }
    case WM_CREATE:
    {
        OnCreate();
        return 0;
    }
    case WM_SIZE:
    {
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam)==0)
        {
            switch (LOWORD(wParam))
            {
                case ID_LOGIN_ENTRY:
                {
                    // Load auth
                    ShowWindow(authWin.Window(), SW_SHOW);//gCmdShow);
                    return 0;
                    break;
                }
                case ID_PASSWORD_ENTRY:
                {
                            MessageBox(m_hwnd,L"Not implemented yet",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                }
                case MENU_EXIT_ITEM:
                {
                    PostQuitMessage(0);
                    break;
                }

            }// switch
        } // if
        return 0;
    }
    //case WM_CLOSE:
    //{
    // PostQuitMessage(0);
    //    return 0;
    //}
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        OnPaint(hdc, ps);
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        return 0;

    }
    return TRUE;
}

