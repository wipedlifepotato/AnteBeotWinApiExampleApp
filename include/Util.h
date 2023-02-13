#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include<string>
#include<iostream>
#include<windows.h>
#define MAX_EDIT_SIZE 1024

namespace Util
{
namespace Edit
{
std::wstring getText(const HWND & edit);
}
namespace Encoding {
std::string UTF8_to_CP1251(std::string const & utf8);
std::string cp1251_to_utf8(const char *str);
}
}
