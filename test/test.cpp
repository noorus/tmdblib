#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>
#include "TMDb.h"

void printVector( const std::wstring& name, const TMDb::StringVector& v )
{
  wprintf_s( L"%s:\r\n", name.c_str() );
  for ( TMDb::StringVector::const_iterator it = v.begin(); it != v.end(); ++it )
    wprintf_s( L"  %s\r\n", (*it).c_str() );
}

int wmain( int argc, WCHAR* argv[] )
{
  try {
    TMDb::TMDb* tmdb = new TMDb::TMDb( L"api-key-here" );
    wprintf_s( L"Base URL: %s\r\n",
      tmdb->getConfiguration().baseURL.c_str() );
    printVector( L"Poster Sizes", tmdb->getConfiguration().posterSizes );
    tmdb->getMovie( 49026 );
    delete tmdb;
  } catch ( std::exception& e ) {
    wprintf_s( L"Exception: %S\r\n", e.what() );
  }
	return EXIT_SUCCESS;
}