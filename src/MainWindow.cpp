#include"MainWindow.h"
//#include<stdio.h>
#include<iostream>
#include<string>
#include<strsafe.h>

//#include <stdafx.h>



// #include <curl/curl.h> // is bullshit. (10+ depencies libraries)
//#include<fstream>
#include"Util.h"
#include<jansson.h>
#include<fstream>
#include<sstream>
#include<winuser.h>
//#define ID_MYBUTTON_ 2
//#define ID_MYBUTTON__ 3
// static unsigned int counter = 0;

//static   HINTERNET  hSession = NULL;
             //hConnect = NULL,
             //hRequest = NULL;

// static HWND loginEdit, passwordEdit;
/*
static CURL * getCurl()
{
    CURL * c =  curl_easy_init();
    assert(c);
    // https://curl.se/libcurl/c/CURLOPT_COOKIEJAR.html
    curl_easy_setopt(c, CURLOPT_COOKIEFILE, "cookies.dat");
    curl_easy_setopt(c, CURLOPT_COOKIEJAR, "cookies.dat");
    //  curl_easy_setopt(curl, CURLOPT_COOKIE, "name=xxx; name2=xxx;");

    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(c, CURLOPT_USERAGENT, "SimpleClientAntebeot 0.0.0");
    return c;
}
*/

std::string MainWindow::getPage(const wchar_t * page, const wchar_t * domain)
{
    std::string ret = "";
    initWinHTTPSession();
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;

    HINTERNET hConnect = WinHttpConnect( m_hSession, domain,
                               INTERNET_DEFAULT_HTTPS_PORT, 0 );
    if (!hConnect)
    {
        MessageBox(m_hwnd,L"Не возможно подключится по адресу",L"Error Box",MB_OK);
        return ret;
    }



    HINTERNET hRequest = WinHttpOpenRequest( hConnect, L"GET", page,
                                   NULL, WINHTTP_NO_REFERER,
                                   NULL, // WINHTTP_DEFAULT_ACCEPT_TYPES
                                   WINHTTP_FLAG_SECURE ); // https://learn.microsoft.com/en-us/windows/win32/winhttp/winhttp-sessions-overview#using-the-winhttp-api-to-access-the-web

  if( !hRequest )
  {
        MessageBox(m_hwnd,L"Не возможно сделать запрос",L"Error Box",MB_OK);
        return ret;

  }
    BOOL   bResults = WinHttpSendRequest( hRequest,
                                   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                   WINHTTP_NO_REQUEST_DATA, 0,
                                   0, 0 );


  // End the request.
  if( !bResults )
  {
        MessageBox(m_hwnd,L"Не возможно полузить результат",L"Error Box",MB_OK);
        return ret;

  }
    bResults = WinHttpReceiveResponse( hRequest, NULL );

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
      if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) ) {
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

        if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
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
  if( hRequest ) WinHttpCloseHandle( hRequest );
  if( hConnect ) WinHttpCloseHandle( hConnect );
  std::cout << "ret: " << ss.str() << std::endl;
  return ss.str();
}
BOOL MainWindow::doAuth(const wchar_t * name, const wchar_t * pass, const wchar_t *otp, const wchar_t * captcha)
{
    //wchar_t page[1024];
//    wsprintfA(page, _T(L"/restapi/signin/?workname=%s&workpass=%s&code=%s&captchaText=%s"), name, pass, otp, captcha);
    std::wstringstream p ;//%s&workpass=%s&code=%s&captchaText=%s" ;
    p << L"/restapi/signin/?workname=";
    p << name; p << "&workpass=";
    p << pass; p << "&code=";
    p << otp; p << "&captchaText=";
    p << captcha; p << "&lang=en_US";
    //p += name;
    //p += "&"
    //std::wcout << "get page: " << p.str() << std::endl;//
    auto res = getPage(p.str().c_str());
    //std::wstring wres(res.begin(), res.end());
    //auto cp1251 = Util::Encoding::cp1251_to_utf8(res.c_str());
    //std::cout << "Result: " << cp1251 << std::endl;
    //MessageBox(m_hwnd,wres.c_str(),L"Error Box",MB_OK);
}
void MainWindow::getCaptcha(const wchar_t *fname)
{

    initWinHTTPSession();
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
// L"https://antebeot.ru/restapi/captcha?w=get",
    // std::wcout << url << std::endl;
    HINTERNET hConnect = WinHttpConnect( m_hSession, defWebDomain,
                               INTERNET_DEFAULT_HTTPS_PORT, 0 );
    if (!hConnect)
    {
        MessageBox(m_hwnd,L"Не возможно подключится по адресу",L"Error Box",MB_OK);
        return;
    }
//TCHAR media_types[10][20];

//for(int i=0; i<10; i++)
//{
//    ZeroMemory(media_types[i], 20);
//}
//StringCchCopy(media_types[0], 20, L"text/html");
//StringCchCopy(media_types[1], 20, L"image/png");
//StringCchCopy(media_types[2], 20, L"image/gif");


    HINTERNET hRequest = WinHttpOpenRequest( hConnect, L"GET", L"/restapi/captcha/?w=get",
                                   NULL, WINHTTP_NO_REFERER,
                                   NULL, // WINHTTP_DEFAULT_ACCEPT_TYPES
                                   WINHTTP_FLAG_SECURE ); // https://learn.microsoft.com/en-us/windows/win32/winhttp/winhttp-sessions-overview#using-the-winhttp-api-to-access-the-web

  if( !hRequest )
  {
        MessageBox(m_hwnd,L"Не возможно сделать запрос",L"Error Box",MB_OK);
        return;

  }
    BOOL   bResults = WinHttpSendRequest( hRequest,
                                   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                   WINHTTP_NO_REQUEST_DATA, 0,
                                   0, 0 );


  // End the request.
  if( !bResults )
  {
        MessageBox(m_hwnd,L"Не возможно полузить результат",L"Error Box",MB_OK);
        return;

  }
    bResults = WinHttpReceiveResponse( hRequest, NULL );

  // Keep checking for data until there is nothing left.
  FILE * pFile;
  pFile = _wfopen(fname, L"w+b"); // NEW
  if( bResults )
  {
    do
    {
      // Check for available data.
      dwSize = 0;
      if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) ) {
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

        if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
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
  if( hRequest ) WinHttpCloseHandle( hRequest );
  if( hConnect ) WinHttpCloseHandle( hConnect );
  // if( hSession ) WinHttpCloseHandle( hSession ); // session need in future

    /*
    CURL *image = getCurl();
    CURLcode imgresult;
    FILE *fp = nullptr;

    {
        // auto temp_path = std::filesystem::temp_directory_path().wstring(); // bullshit

        // Open file
        fp = fopen(fname, "wb");
        if (fp == NULL) std::cerr << "File cannot be opened";
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(image, CURLOPT_URL, url);
        // Grab image
        imgresult = curl_easy_perform(image);
        if (imgresult)
            std::cerr << "Cannot grab a captcha!\n";
    }
    // Clean up the resources
    curl_easy_cleanup(image);
    // Close the file
    fclose(fp);
//    return 0;
    */
}

VOID MainWindow::OnCreate(void)
{
    loginEdit = CreateWindow(L"edit",L"Username",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                             150,25,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
    passwordEdit = CreateWindow(L"edit",L"Пароль",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                                150,50,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
                                //OTPCodeEdit, captchaTextEdit
    OTPCodeEdit = CreateWindow(L"edit",L"2Фа",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                                150,75,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
    captchaTextEdit = CreateWindow(L"edit",L"Каптча",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                                150,100,100,20,m_hwnd,(HMENU) 667,NULL,NULL);
    CreateWindow(L"button",L"Авторизироваться",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
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
    /*
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
    			rect.top=125;
                DrawText(hdc,
    				L"Пароль:",
    				-1,
    				&rect,
    				DT_SINGLELINE | DT_NOCLIP
    			);
    			//DeleteDC(wdc);
    			//
        */
    Graphics graphics(hdc);

    SolidBrush  brush(Color(255, 0, 101, 255));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 14, FontStyleRegular, UnitPixel);
    PointF      pointF(20.0f, 25.0f);

    graphics.DrawString(L"Имя пользователя:", -1, &font, pointF, &brush);
    pointF.Y= pointF.Y + 25;
    graphics.DrawString(L"Пароль:", -1, &font, pointF, &brush);
    pointF.Y= pointF.Y + 25;
    graphics.DrawString(L"2фа:", -1, &font, pointF, &brush);
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


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
        OnCreate();
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
                auto loginText = Util::Edit::getText(loginEdit);
                auto passwordText = Util::Edit::getText(passwordEdit);
                auto OTPCodeText = Util::Edit::getText(OTPCodeEdit);
                auto captchaText = Util::Edit::getText(captchaTextEdit);
                MessageBox(m_hwnd,(L"Имя пользователя: " + loginText).c_str(),L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                MessageBox(m_hwnd,(L"Пароль пользователя: " + passwordText).c_str(),L"ИнфаБокс",MB_OK|MB_ICONWARNING);
                doAuth(loginText.c_str(), passwordText.c_str(), OTPCodeText.c_str(), captchaText.c_str());
                break;
            }
            case ID_CAPTCHAREDRAWBUT:
                {
                    getCaptcha(L"captcha.png"); // L"https://antebeot.ru/restapi/captcha/?w=get",
                    // SendMessage(m_hwnd, WM_PAINT, 0, 0);
                    //InvalidateRect()
                    RedrawWindow(m_hwnd, NULL, NULL,
                             RDW_INVALIDATE | RDW_FRAME | RDW_ERASE | RDW_ALLCHILDREN);
                    break;
                }
            case MENU_EXIT_ITEM:
            {
                PostQuitMessage(0);
                break;
            }

            }
        }
        return 0;

    case WM_CLOSE:
        /*if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
        	MessageBox(hwnd, L"неа нихуя?", L"My application", 0);
        	DestroyWindow(hwnd);
        }*/
        // Else: User canceled. Do nothing.
        //return 0;
        // DestroyWindow(m_hwnd);
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        OnPaint(hdc, ps);
        EndPaint(m_hwnd, &ps);
        //if (counter > 255) counter = 0;

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

