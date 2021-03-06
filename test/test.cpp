#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>
#include "TMDb.h"

void printMovie( TMDb::Movie& movie )
{
  if ( movie.hasField( TMDb::Movie::field_IMDbID ) )
    wprintf_s( L"IMDb ID: %s\r\n", movie.getIMDbID().c_str() );
  if ( movie.hasField( TMDb::Movie::field_Title ) )
    wprintf_s( L"Title: %s\r\n", movie.getTitle().c_str() );
  if ( movie.hasField( TMDb::Movie::field_Overview ) )
    wprintf_s( L"Overview: %s\r\n", movie.getOverview().c_str() );
  if ( movie.hasField( TMDb::Movie::field_Collections ) && !movie.getCollections().empty() )
  {
    wprintf_s( L"In collections:\r\n" );
    for ( TMDb::CollectionMap::const_iterator it = movie.getCollections().begin(); it != movie.getCollections().end(); ++it )
      wprintf_s( L"  %d - %s\r\n", (*it).second.id, (*it).second.name.c_str() );
  }
  if ( movie.hasField( TMDb::Movie::field_Genres ) && !movie.getGenres().empty() )
  {
    wprintf_s( L"Genres:\r\n" );
    for ( TMDb::GenreMap::const_iterator it = movie.getGenres().begin(); it != movie.getGenres().end(); ++it )
      wprintf_s( L"  %d - %s\r\n", (*it).second.id, (*it).second.name.c_str() );
  }
  if ( movie.hasField( TMDb::Movie::field_ProductionCompanies ) && !movie.getProductionCompanies().empty() )
  {
    wprintf_s( L"Production Companies:\r\n" );
    for ( TMDb::CompanyMap::const_iterator it = movie.getProductionCompanies().begin(); it != movie.getProductionCompanies().end(); ++it )
      wprintf_s( L"  %d - %s\r\n", (*it).second.getID(), (*it).second.getName().c_str() );
  }
  if ( movie.hasField( TMDb::Movie::field_ProductionCountries ) && !movie.getProductionCountries().empty() )
  {
    wprintf_s( L"Production Countries: " );
    for ( TMDb::CountryMap::const_iterator it = movie.getProductionCountries().begin(); it != movie.getProductionCountries().end(); ++it )
      wprintf_s( L"%s\r\n", (*it).second.code.c_str(), (*it).second.name.c_str() );
  }
  if ( movie.hasField( TMDb::Movie::field_SpokenLanguages ) && !movie.getSpokenLanguages().empty() )
  {
    wprintf_s( L"Spoken Languages:\r\n" );
    for ( TMDb::LanguageMap::const_iterator it = movie.getSpokenLanguages().begin(); it != movie.getSpokenLanguages().end(); ++it )
      wprintf_s( L"  %s - %s\r\n", (*it).second.code.c_str(), (*it).second.name.c_str() );
  }
}

int wmain( int argc, WCHAR* argv[] )
{
  try {
    TMDb::TMDb* tmdb = new TMDb::TMDb( L"tmdb-api-key-here" );
    // Movie example ---------------
    // TMDb::Movie movie = tmdb->getLatestMovie();
    // wprintf_s( L"Latest Movie - %d\r\n", movie.getID() );
    // printMovie( movie );
    // Company example -------------
    TMDb::Company company = tmdb->getCompany( 15 );
    wprintf_s( L"Movies by %s:\r\n", company.getName().c_str() );
    TMDb::PagedMovieResults& results = tmdb->getCompanyMovies( company.getID() );
    for ( std::vector<TMDb::Movie>::const_iterator it = results.results.begin(); it != results.results.end(); ++it )
    {
      if ( !(*it).hasField( TMDb::Movie::field_Title ) )
        continue;
      if ( (*it).hasField( TMDb::Movie::field_ReleaseDate ) )
        wprintf_s( L"- %s (%d)\r\n", (*it).getTitle().c_str(), (*it).getReleaseDate().year() );
      else
        wprintf_s( L"- %s\r\n", (*it).getTitle().c_str() );
    }
    delete tmdb;
  } catch ( std::exception& e ) {
    wprintf_s( L"Exception: %S\r\n", e.what() );
  }
	return EXIT_SUCCESS;
}