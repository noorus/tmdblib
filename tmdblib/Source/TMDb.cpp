#include "StdAfx.h"
#include "TMDb.h"

namespace TMDb {

  TMDb::TMDb( const std::wstring& apiKey ): mAPIKey( apiKey )
  {
    Poco::Net::initializeNetwork();
    fetchConfig();
  }

  void TMDb::fetchConfig()
  {
    JSONClient client( L"api.themoviedb.org", 80 );
    client.request( L"http://api.themoviedb.org/3/movie/latest?api_key=lols" );
  }

  TMDb::~TMDb()
  {
    Poco::Net::uninitializeNetwork();
  }

}