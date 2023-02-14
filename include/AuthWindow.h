#ifndef AuthWindow_H
#define AuthWindow_H
#include"BaseWindow.h"
#define ID_MYBUTTON 1
#define ID_CAPTCHAREDRAWBUT 2
#define MENU_EXIT_ITEM 1000


// TODO: to macros/template simillar windows. but for now is ok. even double code for now is ok.
class AuthWindow : public BaseWindow<AuthWindow>
{
private:
    BOOL doAuth(const wchar_t * name, const wchar_t * pass, const wchar_t *otp, const wchar_t * captcha);
    HWND  loginEdit, passwordEdit, OTPCodeEdit, captchaTextEdit;
    VOID OnCreate(void);

public:
    PCWSTR  ClassName() const
    {
        return L"AuthAntebeotClass";
    }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // AuthWindow_H
