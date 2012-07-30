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

namespace TMDb {

  using std::vector;
  using std::string;
  using std::wstring;

  typedef vector<wstring> StringVector;

  struct Configuration {
  public:
    wstring baseURL;
    StringVector backdropSizes;
    StringVector posterSizes;
    StringVector profileSizes;
  };

  class Exception: public std::exception {
    //
  };

  class JSONClient {
  protected:
    Poco::Net::HTTPClientSession* mSession;
  public:
    JSONClient( const wstring& host, unsigned short port );
    void request( const wstring& url );
    ~JSONClient();
  };

  class TMDb {
  protected:
    wstring mAPIKey;
    Configuration mConfig;
    void fetchConfig();
  public:
    TMDb( const wstring& apiKey );
    ~TMDb();
  };

}