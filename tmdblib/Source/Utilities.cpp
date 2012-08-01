#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

  using namespace Poco::Net;
  using Poco::URI;
  using std::stringstream;

  string wideToString( const wstring& str, UINT codepage )
  {
    int length = WideCharToMultiByte( codepage, NULL,
      str.c_str(), -1, NULL, NULL, NULL, FALSE );
    if ( length == 0 )
      throw std::exception( "String conversion failed" );
    string result( length - 1, NULL );
    WideCharToMultiByte( codepage, NULL, str.c_str(), -1,
      &result[0], length, NULL, FALSE );
    return result;
  }

  wstring stringToWide( const string& str, UINT codepage )
  {
    int length = MultiByteToWideChar( codepage, NULL,
      str.c_str(), -1, NULL, NULL );
    if ( length == 0 )
      throw std::exception( "String conversion failed" );
    wstring result( length - 1, NULL );
    MultiByteToWideChar( codepage, NULL, str.c_str(), -1,
      &result[0], length );
    return result;
  }

}