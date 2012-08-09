#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

  using namespace Poco::Net;
  using Poco::URI;
  using std::stringstream;

  JSONClient::JSONClient( const wstring& host, unsigned short port ):
  mSession( NULL )
  {
    mSession = new HTTPClientSession( wideToString( host ), port );
  }

  json_spirit::wValue JSONClient::request( const wstring& url )
  {
    URI uri( wideToString( url ) );

    HTTPRequest request( HTTPRequest::HTTP_GET, uri.getPathAndQuery(),
      HTTPMessage::HTTP_1_1 );

    request.add( "Accept", "application/json" );
    request.add( "Accept-Charset", "utf-8" );
    request.add( "Accept-Encoding", "chunked" );
    request.add( "Content-Type", "application/json" );

    mSession->sendRequest( request );

    HTTPResponse response;
    std::istream& stream = mSession->receiveResponse( response );

    if ( response.getStatus() != 200 )
    {
      mSession->reset();
      throw Exception( static_cast<stringstream const&>(
        stringstream() << string( "Bad HTTP response to JSON request: " ) <<
        response.getStatus() << " " << response.getReason() ).str().c_str() );
    }

    wstring bodyStr = stringToWide( static_cast<stringstream const&>(
      stringstream() << stream.rdbuf() ).str() );

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