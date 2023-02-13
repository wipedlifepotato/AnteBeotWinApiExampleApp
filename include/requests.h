#ifndef REQUESTS_H
#define REQUESTS_H


int convertURLtofname (TCHAR * szURL, TCHAR * szname );
int determinepathfilename (TCHAR * szURL, TCHAR * szpath, TCHAR * szname, TCHAR * szpathfilename);
bool GetFile (HINTERNET hOpen, // Handle from InternetOpen()
                    TCHAR *szURL,        // Full URL
                    TCHAR * szpath,
                    TCHAR * szname);
int DownloadURLImage (TCHAR * szURL, TCHAR * szpath, TCHAR * szname);
#endif // REQUESTS_H
