#include"RegisterWindow.h"
#include"Util.h"
#include <jansson.h>
// WARNING: to macros/template for all windows simillar maybe. for now is ok. but double of code.
BOOL RegisterWindow::doRegister(const wchar_t * name, const wchar_t * pass, const wchar_t *pass2, const wchar_t * captcha)
{
    //wchar_t page[1024];
//    wsprintfA(page, _T(L"/restapi/signin/?workname=%s&workpass=%s&code=%s&captchaText=%s"), name, pass, otp, captcha);
    std::wstringstream p ;//%s&workpass=%s&code=%s&captchaText=%s" ;
    p << L"/restapi/registration/?workname=";
    p << name; p << "&workpass=";
    p << pass; p << "&workpass2=";
    p << pass2; p << "&captchaText=";
    p << captcha; p << "&lang=en_US";
    //p += name;
    //p += "&"
    //std::wcout << "get page: " << p.str() << std::endl;//
    auto res = getPage(p.str().c_str());
    //std::wstring wres(res.begin(), res.end());
    //auto cp1251 = Util::Encoding::cp1251_to_utf8(res.c_str());
    //std::cout << "Result: " << cp1251 << std::endl;
    //
    json_t *root;
    json_error_t error;
    root = json_loads(res.c_str(), 0, &error);
    if(!root)
    {
        std::wstringstream err;
        err << "error: on line ";
        err << error.line;
        err << ", " << error.text;
        //wsprintf(errBuf, "error: on line %d: %s\n", error.line, error.text)
        MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK|MB_ICONERROR);
        return FALSE;
    }
    BOOL ret = FALSE;
    json_t *result, *reason;
    result = json_object_get(root, "result");
    reason = json_object_get(root, "reason");

    if (!json_is_boolean(result))
    {
                std::wstringstream err;
                err << "Error result not boolean" << std::endl;
                MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK|MB_ICONERROR);
    }
    else
    {
        if (json_is_true(result)) ret = TRUE;
        else {
                    if(!json_is_string(reason))
                    {
                                std::wstringstream err;
                                err << "Error reason not str" << std::endl;
                                MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK|MB_ICONERROR);
                    }
                    else
                    {
                        std::wstringstream r;
                        auto msg = json_string_value(reason);
                        r << msg;
                        MessageBox(m_hwnd,r.str().c_str(),L"Error Box",MB_OK|MB_ICONERROR);
                    }
        }// if ret not true
    } // if json is boolean ends
    json_decref(root);
    return ret;

}

VOID RegisterWindow::OnCreate(void)
{
    loginEdit = CreateWindow(L"edit",L"Username",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                             150,25,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
    passwordEdit = CreateWindow(L"edit",L"Password",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|ES_PASSWORD,
                                150,50,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
                                //OTPCodeEdit, captchaTextEdit
    passwordTwoEdit = CreateWindow(L"edit",L"Password",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE|ES_PASSWORD,
                                150,75,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
    captchaTextEdit = CreateWindow(L"edit",L"captcha",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                                150,100,110,20,m_hwnd,(HMENU) 667,NULL,NULL);
    CreateWindow(L"button",L"Зарегестрироваться",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                 50,350,150,20,m_hwnd,(HMENU) ID_MYBUTTON,NULL,NULL);
    CreateWindow(L"button",L"Перерисовать капчу",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                 50,250,150,20,m_hwnd,(HMENU) ID_CAPTCHAREDRAWBUT,NULL,NULL);
    getCaptcha(L"captcha.png");


}

static VOID OnPaint(HDC hdc, PAINTSTRUCT & ps)
{
    // All painting occurs here, between BeginPaint and EndPaint.
    ps.fErase = TRUE;
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
//
    Graphics graphics(hdc);

    SolidBrush  brush(Color(255, 0, 101, 255));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 14, FontStyleRegular, UnitPixel);
    PointF      pointF(20.0f, 25.0f);

    graphics.DrawString(L"Имя пользователя:", -1, &font, pointF, &brush);
    pointF.Y= pointF.Y + 25;
    graphics.DrawString(L"Пароль:", -1, &font, pointF, &brush);
    pointF.Y= pointF.Y + 25;
    graphics.DrawString(L"Подтвердить Пароль:", -1, &font, pointF, &brush);
    pointF.Y= pointF.Y + 25;
    graphics.DrawString(L"каптча:", -1, &font, pointF, &brush);
// https://learn.microsoft.com/ru-ru/windows/win32/gdiplus/-gdiplus-cropping-and-scaling-images-use
    //
    Image image(L"captcha.png");
    graphics.DrawImage(&image, 25, 150);
    // 200x150

//   Pen      pen(Color(255, 0, 0, 000));
//   graphics.DrawLine(&pen, 0, 0, 320, 320);

}


LRESULT RegisterWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;

    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
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
                case ID_MYBUTTON:
                {
                    auto loginText = Util::Edit::getText(loginEdit);
                    auto passwordText = Util::Edit::getText(passwordEdit);
                    auto passwordTwoText = Util::Edit::getText(passwordTwoEdit);
                    auto captchaText = Util::Edit::getText(captchaTextEdit);
                    //MessageBox(m_hwnd,(L"Имя пользователя: " + loginText).c_str(),L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    //MessageBox(m_hwnd,(L"Пароль пользователя: " + passwordText).c_str(),L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                    if (doRegister(loginText.c_str(), passwordText.c_str(), passwordTwoText.c_str(), captchaText.c_str()))
                    {
                        changeLanguage();
                        std::cout << "succ, save cookie.dat" << std::endl;
                        displayCookies();
                        updateSession();
                        MessageBox(m_hwnd, L"Перезапустите приложение", L"Info", MB_OK|MB_ICONQUESTION);
                        PostQuitMessage(0);
                    }
                    break;
                }
                case ID_CAPTCHAREDRAWBUT:
                    {
                        getCaptcha(L"captcha.png"); // L"https://antebeot.ru/restapi/captcha/?w=get",
                        // SendMessage(m_hwnd, WM_PAINT, 0, 0);
                        //InvalidateRect()
                        displayCookies();
                        RedrawWindow(m_hwnd, NULL, NULL,
                                 RDW_INVALIDATE | RDW_FRAME | RDW_ERASE | RDW_ALLCHILDREN);
                        break;
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
    case WM_CLOSE:
    {
        //PostQuitMessage(0);
        // https://learn.microsoft.com/ru-ru/windows/win32/api/winuser/nf-winuser-showwindow
        ShowWindow(m_hwnd, SW_HIDE);
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        OnPaint(hdc, ps);
        EndPaint(m_hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        fprintf(stdout,"%ld %ld\n", pt.x, pt.y);
    }
    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        return 0;

    }
    return TRUE;
}

