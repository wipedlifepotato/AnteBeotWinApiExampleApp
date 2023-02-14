#include"MainWindow.h"
#include"Util.h"
#include"AuthWindow.h"
#include"RegisterWindow.h"
//#include<thread>
static AuthWindow authWin;
static RegisterWindow regWin;
//static HWND mList;
// https://antebeot.ru/restapi/en/ApiReference.html
VOID MainWindow::OnCreate(void)
{
    //https://learn.microsoft.com/en-us/windows/win32/recovery/registering-for-application-restart
        if (!updateSession())
        {
            CreateWindow(L"button",L"АВТОРИЗАЦИЯ",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     0,0,150,20,m_hwnd,(HMENU) ID_LOGIN_ENTRY,NULL,NULL);
            CreateWindow(L"button",L"РЕГИСТРАЦИЯ",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     0,50,150,20,m_hwnd,(HMENU) ID_PASSWORD_ENTRY,NULL,NULL);
            assert(authWin.Create(L"Authentiphication", WS_OVERLAPPEDWINDOW));
            assert(regWin.Create(L"Registration", WS_OVERLAPPEDWINDOW));
        }
        else
        {
            MessageBox(m_hwnd, L"Сессия была восстановлена", L"About session", MB_OK|MB_ICONQUESTION);
            CreateWindow(L"button",L"DUMMY",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                 0,0,150,20,m_hwnd,(HMENU) ID_LOGIN_ENTRY,NULL,NULL);
            std::cout << "mList" << std::endl;
            //mList = Util::Windows::CreateListView(m_hwnd);
            //ShowWindow(mList, SW_SHOW);
            //Util::Windows::SetView(mList, SW_SHOW);
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
    case WM_INITDIALOG:
    {
        std::cout << "WM InitDialog" << std::endl;
        break;
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
                }
                case ID_PASSWORD_ENTRY:
                {
                            ShowWindow(regWin.Window(), SW_SHOW);//gCmdShow);
                            //MessageBox(m_hwnd,L"Not implemented yet",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                            return 0;
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

