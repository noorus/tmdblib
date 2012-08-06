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
  using std::stringstream;
  using std::wstringstream;

  using boost::gregorian::date;

  typedef vector<wstring> StringVector;
  typedef map<wstring,wstring> StringMap;
  
  namespace js = json_spirit;

  class TMDb;
  class Movie;

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
    uint32_t id;
    wstring name;
  };

  typedef map<uint32_t,Genre> GenreMap;

  struct Language {
  public:
    wstring code;
    wstring name;
  };

  typedef map<wstring,Language> LanguageMap;

  struct Company {
  friend class TMDb;
  friend class Movie;
  protected:
    bitset<7> mFieldBits;
    struct Fields {
      wstring description;
      wstring headquarters;
      wstring homepage;
      uint32_t id;
      wstring logoPath;
      wstring name;
      // todo: parent company, is it an ID or what?
    } mFields;
  public:
    Company();
    enum FieldBits: int {
      field_Description = 0,
      field_Headquarters,
      field_Homepage,
      field_ID,
      field_LogoPath,
      field_Name,
      field_ParentCompany
    };
    bool hasField( FieldBits field ) const;
    const wstring& getDescription() const;
    const wstring& getHeadquarters() const;
    const wstring& getHomepage() const;
    uint32_t getID() const;
    const wstring& getLogoPath() const;
    const wstring& getName() const;
  };

  typedef map<uint32_t,Company> CompanyMap;

  struct Country {
  public:
    wstring code;
    wstring name;
  };

  typedef map<wstring,Country> CountryMap;

  struct Collection {
  public:
    wstring backdropPath;
    uint32_t id;
    wstring name;
    wstring posterPath;
  };

  typedef map<uint32_t,Collection> CollectionMap;

  template<class T>
  class PagedResults {
  public:
    vector<T> results;
    uint32_t page;
    uint32_t totalPages;
    uint32_t totalResults;
  };

  typedef PagedResults<Movie> PagedMovieResults;
  typedef PagedResults<Company> PagedCompanyResults;

  class Movie {
  friend class TMDb;
  protected:
    Movie();
    bitset<22> mFieldBits;
    struct Fields {
      bool adult;
      wstring backdropPath;
      uint32_t budget;
      CollectionMap collections;
      GenreMap genres;
      wstring homepage;
      uint32_t id;
      wstring imdbID;
      wstring originalTitle;
      wstring overview;
      double popularity;
      wstring posterPath;
      CompanyMap companies;
      CountryMap countries;
      date releaseDate;
      uint32_t revenue;
      uint32_t runtime;
      LanguageMap languages;
      wstring tagline;
      wstring title;
      float voteAverage;
      uint32_t voteCount;
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
    bool hasField( FieldBits field ) const;
    bool isAdult() const;
    const wstring& getBackdropPath() const;
    uint32_t getBudget() const;
    const CollectionMap& getCollections() const;
    const GenreMap& getGenres() const;
    const wstring& getHomepage() const;
    uint32_t getID() const;
    const wstring& getIMDbID() const;
    const wstring& getOriginalTitle() const;
    const wstring& getOverview() const;
    double getPopularity() const;
    const wstring& getPosterPath() const;
    const CompanyMap& getProductionCompanies() const;
    const CountryMap& getProductionCountries() const;
    const date& getReleaseDate() const;
    uint32_t getRevenue() const;
    uint32_t getRuntime() const;
    const LanguageMap& getSpokenLanguages() const;
    const wstring& getTagline() const;
    const wstring& getTitle() const;
    float getVoteAverage() const;
    uint32_t getVoteCount() const;
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
      Company& company );
    void readJSONProductionCountry( const js::wValue& jsonCountry,
      Country& country );
    void readJSONSpokenLanguage( const js::wValue& jsonLanguage,
      Language& language );
    void readJSONCollection( const js::wValue& jsonCollection,
      Collection& collection );
    void readJSONPagedMovieResults( const js::wValue& jsonResults,
      PagedMovieResults& results );
    void readJSONPagedCompanyResults( const js::wValue& jsonResults,
      PagedCompanyResults& results );
    wstring makeURL( const wstring& method, StringMap* query = NULL );
    static const wstring mAPIHost;
  public:
    explicit TMDb( const wstring& apiKey );
    ~TMDb();
    // Configuration
    const Configuration& getConfiguration();
    // Movies
    Movie getMovie( uint32_t id );
    Movie getLatestMovie();
    PagedMovieResults getUpcomingMovies( uint32_t page = 1 );
    PagedMovieResults getNowPlayingMovies( uint32_t page = 1 );
    PagedMovieResults getPopularMovies( uint32_t page = 1 );
    PagedMovieResults getTopRatedMovies( uint32_t page = 1 );
    PagedMovieResults getCompanyMovies( uint32_t company, uint32_t page = 1 );
    // Companies
    Company getCompany( uint32_t id );
    // Search
    PagedMovieResults searchMovies( const wstring& query, bool adult = true,
      uint32_t year = 0, uint32_t page = 1 );
    PagedCompanyResults searchCompanies( const wstring& query, uint32_t page = 1 );
  };

}