#ifndef ABSTRACTWINDOW_H
#define ABSTRACTWINDOW_H
#ifndef UNICODE
#define UNICODE
#endif
//#define CLIENT_NAME L"Let Wall Go Out"
//#define MAIN_WIN_CLASS_NAME L"MainWin"
#include <windows.h>
#include<stdio.h>
#include<windowsx.h>
#include<assert.h>
#include<string>
#include<map>
constexpr wchar_t * ClientName = L"Let Wall Go Out";
constexpr wchar_t * MainWinClassName = L"MainWin";

//typename menuElementType = std::map<std::string, int>;
template <typename DerivedType>
class AbstractWindow
{
    protected:
     void initWinClass(HINSTANCE hInstance)
     {

            // Register the window class.
            //const wchar_t CLASS_NAME[]  = MAIN_WIN_CLASS_NAME;

            WNDCLASS wc = { };

            wc.lpfnWndProc   = DerivedType::mCallback;
            wc.hInstance     = hInstance;
            wc.lpszClassName = AbstractWindow::m_className.c_str();
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
        //    wc.lpszMenuName =  MAKEINTRESOURCE(IDM_MYMENURESOURCE);

            RegisterClass(&wc);//

     }
     std::wstring m_className;
     HWND m_HWND;
     HINSTANCE mHistance;
    public:
        HWND getMWHND()
        {
            return m_HWND;
        }
        void changeSysColor(int r, int g, int b);
        void initMenu(HWND hwnd);
        HWND initWin(HINSTANCE hInstance, const wchar_t * title, const wchar_t * ClassName = MainWinClassName);

        AbstractWindow(void ) =default;
        ~AbstractWindow(void) = default;
        virtual LRESULT CALLBACK mCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0; // m_WHND
        LRESULT CALLBACK defCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT CALLBACK defWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
                DerivedType *pThis = NULL;
                if (uMsg == WM_NCCREATE)
                {
                    CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                    pThis = (DerivedType*)pCreate->lpCreateParams;
                    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                    pThis->m_hwnd = hwnd;
                }
                else
                {
                    pThis = (DerivedType*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                }
                if (pThis)
                {
                    return pThis->mCallback(uMsg, wParam, lParam);
                }
                else
                {
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
                }
        }

        //DerivedType * getMThis(void) {
        //    return (DerivedType*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        //}
    protected:
        HWND m_hwnd;
    private:
        // WNDCLASS mWc;
        //DerivedType * m_This = NULL;

};

#endif // ABSTRACTWINDOW_H
