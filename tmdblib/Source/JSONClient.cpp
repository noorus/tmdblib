#include "StdAfx.h"
#include "TMDb.h"

namespace TMDb {

  using namespace Poco::Net;
  using Poco::URI;

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

  JSONClient::JSONClient( const wstring& host, unsigned short port ):
  mSession( NULL )
  {
    mSession = new HTTPClientSession( wideToString( host, CP_UTF8 ), port );
  }

  void JSONClient::request( const wstring& url )
  {
    URI uri( wideToString( url, CP_UTF8 ) );
    HTTPRequest request(
      HTTPRequest::HTTP_GET,
      uri.getPathAndQuery(),
      HTTPMessage::HTTP_1_1 );
    request.add( "Accept", "application/json" );
    request.add( "Accept-Charset", "utf-8" );
    request.add( "Accept-Encoding", "chunked" );
    mSession->sendRequest( request );
    HTTPResponse response;
    std::istream& stream = mSession->receiveResponse( response );
    wstring body = stringToWide(
      static_cast<std::stringstream const&>( std::stringstream() << stream.rdbuf() ).str(),
      CP_UTF8 );
    wprintf_s( L"%s", body.c_str() );
  }

  JSONClient::~JSONClient()
  {
    if ( mSession )
      delete mSession;
  }

}