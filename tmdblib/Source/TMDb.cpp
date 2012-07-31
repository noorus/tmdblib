#include "StdAfx.h"
#include "TMDb.h"

namespace TMDb {

  const wstring TMDb::mAPIHost( L"api.themoviedb.org" );

  namespace js = json_spirit;

  TMDb::TMDb( const std::wstring& apiKey ): mAPIKey( apiKey ), mClient( NULL )
  {
    Poco::Net::initializeNetwork();
    mClient = new JSONClient( mAPIHost, 80 );
    fetchConfiguration();
  }

  wstring TMDb::makeURL( LPCWSTR format, StringMap* query, ... )
  {
    va_list va_alist;
    std::wstring str( 512, NULL );
    va_start( va_alist, format );
    va_arg( va_alist, int ); // Skip query arg
    _vsnwprintf_s( &str[0], 512, 511, format, va_alist );
    va_end( va_alist );

    wstring url( 512, NULL );
    swprintf_s( &url[0], 512, L"http://%s/3/%s?api_key=%s",
      mAPIHost.c_str(), str.c_str(), mAPIKey.c_str() );

    if ( query )
      for ( StringMap::const_iterator it = query->begin(); it != query->end(); ++it )
      {
        WCHAR qstr[512];
        swprintf_s( qstr, 512, L"&%s=%s", (*it).first.c_str(), (*it).second.c_str() );
        url.append( qstr );
      }
    return url;
  }

  void TMDb::readJSONArray( const json_spirit::wValue& val,
  const wstring& path, StringVector& vec )
  {
    vec.clear();
    js::wArray arr = val.getArray( path );
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
      vec.push_back( (*it).getString() );
  }

  void TMDb::fetchConfiguration()
  {
    js::wValue val = mClient->request( makeURL( L"configuration" ) );

    mConfiguration.baseURL = val.getString( L"images.base_url" );

    readJSONArray( val, L"images.poster_sizes", mConfiguration.posterSizes );
    readJSONArray( val, L"images.backdrop_sizes", mConfiguration.backdropSizes );
    readJSONArray( val, L"images.profile_sizes", mConfiguration.profileSizes );
    readJSONArray( val, L"images.logo_sizes", mConfiguration.logoSizes );
  }

  const Configuration& TMDb::getConfiguration()
  {
    return mConfiguration;
  }

  const Movie& TMDb::getMovie( unsigned int id )
  {
    //js::wValue val = mClient->request( makeURL( L"movie/%d", NULL, id ) );
    wprintf_s( L"%s\r\n", makeURL( L"movie/%d", NULL, id ).c_str() );
    return Movie();
  }

  TMDb::~TMDb()
  {
    if ( mClient )
      delete mClient;
    Poco::Net::uninitializeNetwork();
  }

}