#ifndef RegisterWindow_H
#define RegisterWindow_H
#include"BaseWindow.h"
#define ID_MYBUTTON 1
#define ID_CAPTCHAREDRAWBUT 2
#define MENU_EXIT_ITEM 1000



class RegisterWindow : public BaseWindow<RegisterWindow>
{
private:
    BOOL doRegister(const wchar_t * name, const wchar_t * pass, const wchar_t *otp, const wchar_t * captcha);
    HWND  loginEdit, passwordEdit, passwordTwoEdit, captchaTextEdit;
    VOID OnCreate(void);

public:
    PCWSTR  ClassName() const
    {
        return L"RegAntebeotClass";
    }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // AuthWindow_H
