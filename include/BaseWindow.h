#ifndef UNICODE
#define UNICODE
#endif
#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include<windows.h>
#include<windowsx.h>
#include<assert.h>
#include<iostream>
#include<string>
#include<sstream>
#include<winuser.h>
#include<fstream>
#include<sstream>
#include<strsafe.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
//#include<winhttp.h>
//#pragma comment(lib, "winhttp.lib")
#include <WinInet.h>
#pragma comment(lib,"wininet")
#include<jansson.h>
#include <commctrl.h>

extern int gCmdShow;
extern BOOL progCanBeClosed;
constexpr wchar_t * defWebDomain = L"antebeot.ru";

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    void displayCookies()
    {}
    BOOL getPageOfReasonType(const wchar_t * url, bool showErrorBoxes = true)
    {
        auto res = getPage(url);
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
                MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK);
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
                        if (showErrorBoxes) MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK);
            }
            else
            {
                if (json_is_true(result)) ret = TRUE;
                else {
                            if(!json_is_string(reason))
                            {
                                        std::wstringstream err;
                                        err << "Error reason not str" << std::endl;
                                        if (showErrorBoxes) MessageBox(m_hwnd,err.str().c_str(),L"Error Box",MB_OK);
                            }
                            else
                            {
                                std::wstringstream r;
                                auto msg = json_string_value(reason);
                                r << msg;
                                if (showErrorBoxes) MessageBox(m_hwnd,r.str().c_str(),L"Error Box",MB_OK);
                            }
                }// if ret not true
            } // if json is boolean ends
            json_decref(root);
            return ret;

    }
    BOOL changeLanguage(const wchar_t * lang = L"en_US")
    {
        // /user/?w=changeLanguage&lang=en_US
        std::wstring url(L"/restapi/user/?w=changeLanguage&lang="+std::wstring{lang});
        return getPageOfReasonType(url.c_str(), false);
    }
    BOOL updateSession()
    {
        return getPageOfReasonType(L"/restapi/user/?w=updateSession", false);
    }
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) { }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = 450,
        int nHeight = 450,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
                     dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
                     nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
                 );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Window() const
    {
        return m_hwnd;
    }
public:
    const HWND & getMHWND()
    {
        return m_hwnd;
    }
protected:
     HINTERNET  m_hSession = NULL;

     void initWinHTTPSession(void)
    {
             if (m_hSession == NULL)
             {
                m_hSession = InternetOpen(L"WinHTTP ImageGetter/0.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);//https://learn.microsoft.com/ru-ru/windows/win32/wininet/enabling-internet-functionality
                // by them PDF. is not supports (that below) save cookie.
                /*WinHttpOpen( L"WinHTTP ImageGetter/0.0",
                              WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                              WINHTTP_NO_PROXY_NAME,
                              WINHTTP_NO_PROXY_BYPASS, 0 );*/
                // if (!m_hSession) std::cout << "Session init error" << std::endl;
                //std::cout << "Session was inited" << std::endl;
             } else
             {
                 //std::cout << "Session was inited before" << std::endl;
             }
            assert(m_hSession);
    }
    std::string getPage(const wchar_t * page, const wchar_t * domain = defWebDomain)
    {
            std::string ret = "";
            initWinHTTPSession();
            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            LPSTR pszOutBuffer;

            HINTERNET hConnect = InternetConnect(m_hSession, domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL,  INTERNET_SERVICE_HTTP, 0,  0);
            //WinHttpConnect( m_hSession, domain,
              //                         INTERNET_DEFAULT_HTTPS_PORT, 0 );
            if (!hConnect)
            {
                MessageBox(m_hwnd,L"Не возможно подключится по адресу",L"Error Box",MB_OK);
                return ret;
            }



            HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", page, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_SECURE, 0);
            //WinHttpOpenRequest( hConnect, L"GET", page,
             //                              NULL, WINHTTP_NO_REFERER,
              //                             NULL, // WINHTTP_DEFAULT_ACCEPT_TYPES
              //                             WINHTTP_FLAG_SECURE ); // https://learn.microsoft.com/en-us/windows/win32/winhttp/winhttp-sessions-overview#using-the-winhttp-api-to-access-the-web

          if( !hRequest )
          {
                MessageBox(m_hwnd,L"Не возможно сделать запрос",L"Error Box",MB_OK);
                return ret;

          }
            BOOL   bResults = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
            //WinHttpSendRequest( hRequest,
             //                              WINHTTP_NO_ADDITIONAL_HEADERS, 0,
             //                              WINHTTP_NO_REQUEST_DATA, 0,
              //                             0, 0 );


          // End the request.
          if( !bResults )
          {
                MessageBox(m_hwnd,L"Не возможно полузить результат",L"Error Box",MB_OK);
                return ret;

          }
        // bResults = WinHttpReceiveResponse( hRequest, NULL );

          // Keep checking for data until there is nothing left.
          //FILE * pFile;
          //pFile = _wfopen(fname, L"w+b"); // NEW
          std::stringstream ss;
          if( bResults )
          {
            do
            {
              // Check for available data.
              dwSize = 0;
              // WinHttpQueryDataAvailable
              if( !InternetQueryDataAvailable ( hRequest, &dwSize, 0, 0 ) ) {
                printf( "Error %u in WinHttpQueryDataAvailable.\n",
                        GetLastError( ) );
                 MessageBox(m_hwnd,L"Ошибка при получение данных",L"Error Box",MB_OK);
                 return ret;
              }

              // Allocate space for the buffer.
              pszOutBuffer = new char[dwSize+1];
              if( !pszOutBuffer )
              {
                printf( "Out of memory\n" );
                dwSize=0;
              }
              else
              {
                // Read the data.
                ZeroMemory( pszOutBuffer, dwSize+1 ); // memset(.. 0,
                // WinHttpReadData
                if( !InternetReadFile( hRequest, (LPVOID)pszOutBuffer,
                                      dwSize, &dwDownloaded ) )
                  printf( "Error %u in WinHttpReadData.\n", GetLastError( ) );
                else {
                  // fwrite(pszOutBuffer, (size_t)dwDownloaded, (size_t)1, pFile);
                  ss << pszOutBuffer;
                  //std::string tmp{pszOutBuffer};
                  //ret = ret + tmp;
                  //std::cout << "ret: " << ret << std::endl;
                }

                // Free the memory allocated to the buffer.
                delete [] pszOutBuffer;
              }
            } while( dwSize > 0 );
          }
          //fclose(pFile);


          // Report any errors.
          if( !bResults )
            printf( "Error %d has occurred.\n", GetLastError( ) );

          // Close any open handles.
          if( hRequest ) InternetCloseHandle( hRequest );//WinHttpCloseHandle( hRequest );
          if( hConnect ) InternetCloseHandle( hConnect );
          std::cout << "ret: " << ss.str() << std::endl;
          return ss.str();
    }
    void getCaptcha(const wchar_t *fname)
    {

            initWinHTTPSession();
            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            LPSTR pszOutBuffer;
        // L"https://antebeot.ru/restapi/captcha?w=get",
            // std::wcout << url << std::endl;
            //HINTERNET hConnect = InternetConnect(m_hSession, domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL,  INTERNET_SERVICE_HTTP, 0,  0);
            HINTERNET hConnect = InternetConnect( m_hSession, defWebDomain,
                                       INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0 );
            if (!hConnect)
            {
                MessageBox(m_hwnd,L"Не возможно подключится по адресу",L"Error Box",MB_OK);
                return;
            }

            HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", L"/restapi/captcha/?w=get", NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_SECURE, 0);

          if( !hRequest )
          {
                MessageBox(m_hwnd,L"Не возможно сделать запрос",L"Error Box",MB_OK);
                return;

          }
            BOOL   bResults = HttpSendRequest(hRequest, NULL, 0, NULL, 0);


          // End the request.
          if( !bResults )
          {
                MessageBox(m_hwnd,L"Не возможно полузить результат",L"Error Box",MB_OK);
                return;

          }
            //bResults = WinHttpReceiveResponse( hRequest, NULL );

          // Keep checking for data until there is nothing left.
          FILE * pFile;
          pFile = _wfopen(fname, L"w+b"); // NEW
          if( bResults )
          {
            do
            {
              // Check for available data.
              dwSize = 0;
              if( !InternetQueryDataAvailable( hRequest, &dwSize, 0,0 ) ) {
                printf( "Error %u in WinHttpQueryDataAvailable.\n",
                        GetLastError( ) );
                 MessageBox(m_hwnd,L"Ошибка при получение данных",L"Error Box",MB_OK);
                 return;
              }

              // Allocate space for the buffer.
              pszOutBuffer = new char[dwSize+1];
              if( !pszOutBuffer )
              {
                printf( "Out of memory\n" );
                dwSize=0;
              }
              else
              {
                // Read the data.
                ZeroMemory( pszOutBuffer, dwSize+1 ); // memset(.. 0,

                if( !InternetReadFile( hRequest, (LPVOID)pszOutBuffer,
                                      dwSize, &dwDownloaded ) )
                  printf( "Error %u in WinHttpReadData.\n", GetLastError( ) );
                else {
                  fwrite(pszOutBuffer, (size_t)dwDownloaded, (size_t)1, pFile);
                  //printf( "%s", pszOutBuffer );
                }

                // Free the memory allocated to the buffer.
                delete [] pszOutBuffer;
              }
            } while( dwSize > 0 );
          }
          fclose(pFile);


          // Report any errors.
          if( !bResults )
            printf( "Error %d has occurred.\n", GetLastError( ) );

          // Close any open handles.
          if( hRequest ) InternetCloseHandle( hRequest );
          if( hConnect ) InternetCloseHandle( hConnect );

    }

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};


#endif // BASEWINDOW_H
