#pragma once

#include <exception>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <regex>
#include <stack>

#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPClientSession.h>

#define JSON_SPIRIT_VALUE_ENABLED
#include "../json_spirit/reader_template.h"
#include "../json_spirit/writer_template.h"

namespace TMDb {

  using std::vector;
  using std::string;
  using std::wstring;
  using std::pair;
  using std::map;

  typedef vector<wstring> StringVector;
  typedef map<wstring,wstring> StringMap;

  struct Configuration {
  public:
    wstring baseURL;
    StringVector posterSizes;
    StringVector backdropSizes;
    StringVector profileSizes;
    StringVector logoSizes;
  };

  struct Movie {
  public:
    bool adult;
    wstring backdropPath;
    int budget;
    wstring homepage;
    int id;
    wstring imdbID;
    wstring originalTitle;
    wstring overview;
    float popularity;
    wstring posterPath;
    wstring tagline;
    int runtime;
    wstring title;
    float voteAverage;
    int voteCount;
  };

  class Exception: public std::exception {
    //
  };

  class JSONClient {
  protected:
    Poco::Net::HTTPClientSession* mSession;
  public:
    JSONClient( const wstring& host, unsigned short port );
    json_spirit::wValue request( const wstring& url );
    ~JSONClient();
  };

  class TMDb {
  protected:
    wstring mAPIKey;
    Configuration mConfiguration;
    JSONClient* mClient;
    void fetchConfiguration();
    void readJSONArray( const json_spirit::wValue& val, const wstring& path, StringVector& vec );
    wstring makeURL( LPCWSTR format, StringMap* query = NULL, ... );
    static const wstring mAPIHost;
  public:
    TMDb( const wstring& apiKey );
    const Configuration& getConfiguration();
    const Movie& getMovie( unsigned int id );
    ~TMDb();
  };

}