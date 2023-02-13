#include "AbstractWindow.h"


// -lgdi32
/*static*/


template<class DerivedType> void
AbstractWindow<DerivedType>::changeSysColor(int r, int g, int b)
{
    int aElements[2] = {COLOR_WINDOW, COLOR_ACTIVECAPTION}; // elements for set colors
    COLORREF newColors[] = {RGB( r & 0xff, g & 0xff, b & 0xff )};  // % 255
    SetSysColors(2, aElements, newColors);
}


template<class DerivedType> void
AbstractWindow<DerivedType>::initMenu(HWND hwnd)
{
    HMENU hMainMenu;
    hMainMenu = CreateMenu();
    HMENU hPopMenuFile = CreatePopupMenu();

    AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"Файл");
    AppendMenu(hMainMenu, MF_STRING, 1000, L"Справка");

    AppendMenu(hPopMenuFile, MF_STRING, 1001, L"Отрпавить данные в отдел");

    SetMenu(hwnd, hMainMenu);
    SetMenu(hwnd, hPopMenuFile);
}


template<class DerivedType> HWND
AbstractWindow<DerivedType>::initWin(HINSTANCE hInstance, const wchar_t * title, const wchar_t * ClassName)
{
    HWND hwnd = CreateWindowEx(
                    0,                              // Optional window styles.
                    ClassName,                     // Window class
                    title,    // Window text
                    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,             // Window style

                    // Size and position
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                    NULL,       // Parent window
                    NULL,       // Menu
                    hInstance,  // Instance handle
                    NULL        // Additional application data
                );
    assert(hwnd != NULL);
    initMenu(hwnd);


    return hwnd;
}



#define ID_MYBUTTON 1
#define ID_MYBUTTON_ 2
#define ID_MYBUTTON__ 3
// static unsigned int counter = 0;
static HWND myEdit;
static wchar_t UserNameText[64]; // max buf

// -municode
//https://learn.microsoft.com/ru-ru/windows/win32/api/_winmsg/
template<class DerivedType>
LRESULT CALLBACK  AbstractWindow<DerivedType>::defCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    defWindowProc(hwnd, uMsg, wParam, lParam);
    POINT pt;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
        CreateWindow(L"button",L"My button",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     5,5,100,20,hwnd,(HMENU) ID_MYBUTTON,NULL,NULL);
        CreateWindow(L"button",L"My button",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     100,5,100,20,hwnd,(HMENU) ID_MYBUTTON_,NULL,NULL);
        CreateWindow(L"button",L"My button",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                     200,5,100,20,hwnd,(HMENU) ID_MYBUTTON__,NULL,NULL);
        myEdit = CreateWindow(L"edit",L"Username",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                              200,100,100,20,hwnd,(HMENU) 667,NULL,NULL);
        //SendMessage(myEdit, EM_LIMITTEXT, sizeof( UserNameText ), 0L);
        return 0;
    case WM_SIZE:
        //MoveWindow(myEdit, 0, 20, LOWORD(lParam),
        //HIWORD(lParam) - 20, TRUE);
        break;
    case WM_COMMAND:
        /* нажата наша кнопочка? */
        if (HIWORD(wParam)==0)
        {
            switch (LOWORD(wParam))
            {
                case ID_MYBUTTON:
                {
                    WORD wSize = GetWindowTextLength(myEdit);
                    fprintf(stdout, "WSize: %d\n", wSize);
                    GetWindowText(myEdit, UserNameText, sizeof(UserNameText));
                    fprintf(stdout, "usernametext: %ls\n", UserNameText);
                    // https://www.thinkage.ca/gcos/expl/c/lib/wcstom.html
                    wchar_t buf[sizeof(UserNameText) + sizeof(L"имяпользователя:") + 1];
                    wcscpy(buf, L"имяпользователя:");
                    wcscat(buf, UserNameText);
                    MessageBox(hwnd,buf,L"ИнфаБокс",MB_OK|MB_ICONWARNING);

                    break;
                }
                case ID_MYBUTTON_:
                {
                    MessageBox(hwnd,L"You pressed my button 2",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    break;
                }
                case ID_MYBUTTON__:
                {
                    MessageBox(hwnd,L"You pressed my button 3",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    break;
                }
                case 1000:
                {
                    MessageBox(hwnd,L"You Справка?",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    break;
                }
                case 1001:
                {
                    MessageBox(hwnd,L"Ваши данные были отправлены в отдел?",L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    break;
                }
            }
        }
        return 0;

    case WM_CLOSE:
        /*if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
        	DestroyWindow(hwnd);
        }*/
        // Else: User canceled. Do nothing.
        //return 0;
        DestroyWindow(hwnd);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        ps.fErase = FALSE;
        HDC hdc = BeginPaint(hwnd, &ps);
        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

        // Draw text
        RECT rect = {20, 100, 0,0}; // left,top, right, bottom
        // HDC wdc = GetWindowDC(hwnd);
        //GetClientRect (hwnd, &rect) ;
        SetTextColor(hdc,RGB(128,128,128));
        SetBkMode(hdc,TRANSPARENT);
        DrawText(hdc,
                 L"Юзернейм:",
                 -1,
                 &rect,
                 DT_SINGLELINE | DT_NOCLIP
                );
        //DeleteDC(wdc);
        //
        EndPaint(hwnd, &ps);
        //if (counter > 255) counter = 0;
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        fprintf(stdout,"%d %d\n", pt.x, pt.y);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

        return 0;

    }

}

