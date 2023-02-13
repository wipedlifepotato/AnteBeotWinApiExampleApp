#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"BaseWindow.h"
#define ID_MYBUTTON 1
#define ID_CAPTCHAREDRAWBUT 2
#define MENU_EXIT_ITEM 1000
#include<winhttp.h>
#include <objidl.h>
#include <gdiplus.h>
#include<iostream>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "winhttp.lib")
constexpr wchar_t * defWebDomain = L"antebeot.ru";

class MainWindow : public BaseWindow<MainWindow>
{
private:
    std::string getPage(const wchar_t * page, const wchar_t * domain = defWebDomain);
    BOOL doAuth(const wchar_t * name, const wchar_t * pass, const wchar_t *otp, const wchar_t * captcha);
    HINTERNET  m_hSession = NULL;
    HWND  loginEdit, passwordEdit, OTPCodeEdit, captchaTextEdit;
    void getCaptcha(const wchar_t * url );
    VOID OnCreate(void);
    void initWinHTTPSession(void)
    {
             if (m_hSession == NULL)
             {
                m_hSession = WinHttpOpen( L"WinHTTP ImageGetter/0.0",
                              WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                              WINHTTP_NO_PROXY_NAME,
                              WINHTTP_NO_PROXY_BYPASS, 0 );
                // if (!m_hSession) std::cout << "Session init error" << std::endl;
                //std::cout << "Session was inited" << std::endl;
             } else
             {
                 //std::cout << "Session was inited before" << std::endl;
             }
            assert(m_hSession);
    }
public:
    PCWSTR  ClassName() const
    {
        return L"Sample Window Class";
    }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // MAINWINDOW_H
