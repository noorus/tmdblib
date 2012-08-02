#pragma once

#include <windows.h>

namespace TMDb {

  using std::string;
  using std::wstring;

  string wideToString( const wstring& str, UINT codepage = CP_UTF8 );
  wstring stringToWide( const string& str, UINT codepage = CP_UTF8 );
  wstring widePrintf( LPCWSTR format, ... );

}