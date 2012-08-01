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
#include <bitset>

#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPClientSession.h>

#include <boost/date_time/gregorian/gregorian.hpp>

#define JSON_SPIRIT_VALUE_ENABLED
#include "../json_spirit/reader_template.h"
#include "../json_spirit/writer_template.h"

#include "stdint.h"

namespace TMDb {

  using std::vector;
  using std::string;
  using std::wstring;
  using std::pair;
  using std::map;
  using std::bitset;

  using boost::gregorian::date;

  typedef vector<wstring> StringVector;
  typedef map<wstring,wstring> StringMap;
  
  namespace js = json_spirit;

  struct Configuration {
  public:
    wstring baseURL;
    StringVector posterSizes;
    StringVector backdropSizes;
    StringVector profileSizes;
    StringVector logoSizes;
  };

  struct Genre {
  public:
    int id;
    wstring name;
  };

  typedef map<int,Genre> GenreMap;

  struct Language {
  public:
    wstring code;
    wstring name;
  };

  typedef map<wstring,Language> LanguageMap;

  struct ProductionCompany {
  public:
    int id;
    wstring name;
  };

  typedef map<int,ProductionCompany> ProductionCompanyMap;

  struct ProductionCountry {
  public:
    wstring code;
    wstring name;
  };

  typedef map<wstring,ProductionCountry> ProductionCountryMap;

  class TMDb;

  class Movie {
  friend class TMDb;
  protected:
    Movie();
    bitset<22> mFieldBits;
    struct Fields {
      bool adult;
      wstring backdropPath;
      uint32_t budget;
      GenreMap genres;
      wstring homepage;
      uint32_t id;
      wstring imdbID;
      wstring originalTitle;
      wstring overview;
      float popularity;
      wstring posterPath;
      ProductionCompanyMap companies;
      ProductionCountryMap countries;
      date releaseDate;
      uint32_t revenue;
      uint32_t runtime;
      wstring tagline;
      wstring title;
      float voteAverage;
      uint32_t voteCount;
      Fields();
    } mFields;
  public:
    enum FieldBits: int {
      field_Adult = 0,
      field_Backdrop,
      field_Collections,
      field_Budget,
      field_Genres,
      field_Homepage,
      field_ID,
      field_IMDbID,
      field_OriginalTitle,
      field_Overview,
      field_Popularity,
      field_Poster,
      field_ProductionCompanies,
      field_ProductionCountries,
      field_ReleaseDate,
      field_Revenue,
      field_Runtime,
      field_SpokenLanguages,
      field_Tagline,
      field_Title,
      field_VoteAverage,
      field_VoteCount
    };
    bool hasField( FieldBits field );
    bool isAdult();
    const wstring& getBackdropPath();
    uint32_t getBudget();
    const GenreMap& getGenres();
    const wstring& getHomepage();
    uint32_t getID();
    const wstring& getIMDbID();
    const wstring& getOriginalTitle();
    const wstring& getOverview();
    float getPopularity();
    const wstring& getPosterPath();
    const ProductionCompanyMap& getProductionCompanies();
    const ProductionCountryMap& getProductionCountries();
    const date& getReleaseDate();
    uint32_t getRevenue();
    uint32_t getRuntime();
    const wstring& getTagline();
    const wstring& getTitle();
    float getVoteAverage();
    uint32_t getVoteCount();
  };

  class Exception: public std::exception {
    //
  };

  class JSONClient {
  protected:
    Poco::Net::HTTPClientSession* mSession;
  public:
    JSONClient( const wstring& host, unsigned short port );
    js::wValue request( const wstring& url );
    ~JSONClient();
  };

  class TMDb {
  protected:
    wstring mAPIKey;
    Configuration mConfiguration;
    JSONClient* mClient;
    void fetchConfiguration();
    void readJSONArray( const js::wValue& jsonArray, const wstring& path,
      StringVector& vec );
    void readJSONMovie( const js::wValue& jsonMovie, Movie& movie );
    void readJSONGenre( const js::wValue& jsonGenre, Genre& genre );
    void readJSONProductionCompany( const js::wValue& jsonCompany,
      ProductionCompany& company );
    void readJSONProductionCountry( const js::wValue& jsonCountry,
      ProductionCountry& country );
    wstring makeURL( LPCWSTR format, StringMap* query = NULL, ... );
    static const wstring mAPIHost;
  public:
    TMDb( const wstring& apiKey );
    const Configuration& getConfiguration();
    Movie getMovie( unsigned int id );
    ~TMDb();
  };

}