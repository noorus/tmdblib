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
    TMDb::TMDb* tmdb = new TMDb::TMDb( L"lols" );
    TMDb::Movie movie = tmdb->getMovie( 49026 );
    if ( movie.hasField( TMDb::Movie::field_Title ) )
      wprintf_s( L"Title: %s\r\n", movie.getTitle().c_str() );
    if ( movie.hasField( TMDb::Movie::field_Overview ) )
      wprintf_s( L"Overview: %s\r\n", movie.getOverview().c_str() );
    if ( movie.hasField( TMDb::Movie::field_Genres ) )
    {
      wprintf_s( L"Genres:\r\n" );
      for ( TMDb::GenreMap::const_iterator it = movie.getGenres().begin(); it != movie.getGenres().end(); ++it )
        wprintf_s( L"  %d - %s\r\n", (*it).second.id, (*it).second.name.c_str() );
    }
    delete tmdb;
  } catch ( std::exception& e ) {
    wprintf_s( L"Exception: %S\r\n", e.what() );
  }
	return EXIT_SUCCESS;
}