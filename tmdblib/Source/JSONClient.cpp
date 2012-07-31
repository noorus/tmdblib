#include "StdAfx.h"
#include "TMDb.h"

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

  JSONClient::JSONClient( const wstring& host, unsigned short port ):
  mSession( NULL )
  {
    mSession = new HTTPClientSession( wideToString( host, CP_UTF8 ), port );
  }

  json_spirit::wValue JSONClient::request( const wstring& url )
  {
    URI uri( wideToString( url, CP_UTF8 ) );

    HTTPRequest request(
      HTTPRequest::HTTP_GET,
      uri.getPathAndQuery(),
      HTTPMessage::HTTP_1_1 );
    request.add( "Accept", "application/json" );
    request.add( "Accept-Charset", "utf-8" );
    request.add( "Accept-Encoding", "chunked" );
    request.add( "Content-Type", "application/json" );

    mSession->sendRequest( request );

    HTTPResponse response;
    std::istream& stream = mSession->receiveResponse( response );

    if ( response.getStatus() >= 400 ) {
      mSession->reset();
      throw std::exception( "JSON request failed" );
    }

    wstring bodyStr = stringToWide(
      static_cast<stringstream const&>(stringstream()<<stream.rdbuf()).str(),
      CP_UTF8 );

    json_spirit::wValue bodyVal;
    json_spirit::read_string_or_throw( bodyStr, bodyVal );
    return bodyVal;
  }

  JSONClient::~JSONClient()
  {
    if ( mSession )
      delete mSession;
  }

}