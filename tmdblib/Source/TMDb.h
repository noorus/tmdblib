#pragma once

#include <exception>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <bitset>

#include <Poco/URI.h>
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPClientSession.h>

#include <boost/date_time/gregorian/gregorian.hpp>

#define JSON_SPIRIT_VALUE_ENABLED
#include "../json_spirit/reader_template.h"

namespace TMDb {

  typedef signed __int16 int16_t;
  typedef signed __int32 int32_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int32 uint32_t;

  using std::vector;
  using std::string;
  using std::wstring;
  using std::list;
  using std::pair;
  using std::map;
  using std::bitset;
  using std::stringstream;
  using std::wstringstream;

  using boost::gregorian::date;

  typedef vector<wstring> StringVector;
  typedef map<wstring,wstring> StringMap;
  typedef int FieldIndex;
  
  namespace js = json_spirit;

  template <size_t _fields>
  class FieldBasedClass {
  protected:
    bitset<_fields> mFieldBits;
  public:
    FieldBasedClass()
    {
      mFieldBits.reset();
    }
    bool hasField( FieldIndex field ) const
    {
      if ( field >= mFieldBits.size() )
        throw Exception( "Field does not exist" );
      return mFieldBits[field];
    }
  };

  class TMDb;
  class Movie;

  // Configuration container
  // http://help.themoviedb.org/kb/api/configuration
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

  // Person entry
  // http://help.themoviedb.org/kb/api/person-info
  class Person: public FieldBasedClass<10> {
  friend class TMDb;
  protected:
    struct Fields {
      bool adult;
      // todo: also known as
      wstring biography;
      date birthday;
      date deathday;
      wstring homepage;
      uint32_t id;
      wstring name;
      wstring placeOfBirth;
      wstring profilePath;
    } mFields;
  public:
    enum FieldBits: FieldIndex {
      field_Adult = 0,
      field_AlsoKnownAs,
      field_Biography,
      field_Birthday,
      field_Deathday,
      field_Homepage,
      field_ID,
      field_Name,
      field_PlaceOfBirth,
      field_ProfilePath
    };
    bool isAdult() const;
    const wstring& getBiography() const;
    const date& getBirthday() const;
    const date& getDeathday() const;
    const wstring& getHomepage() const;
    uint32_t getID() const;
    const wstring& getName() const;
    const wstring& getPlaceOfBirth() const;
    const wstring& getProfilePath() const;
  };

  // Company entry
  // http://help.themoviedb.org/kb/api/company-info
  class Company: public FieldBasedClass<7> {
  friend class TMDb;
  friend class Movie;
  protected:
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
    enum FieldBits: FieldIndex {
      field_Description = 0,
      field_Headquarters,
      field_Homepage,
      field_ID,
      field_LogoPath,
      field_Name,
      field_ParentCompany
    };
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
  typedef PagedResults<Person> PagedPersonResults;

  // Image entry
  // http://help.themoviedb.org/kb/api/person-images
  // http://help.themoviedb.org/kb/api/movie-images
  class Image: public FieldBasedClass<5> {
  friend class TMDb;
  protected:
    struct Fields {
      double aspectRatio;
      wstring filePath;
      uint32_t height;
      wstring languageCode;
      uint32_t width;
    } mFields;
  public:
    enum FieldBits: FieldIndex {
      field_AspectRatio = 0,
      field_FilePath,
      field_Height,
      field_LanguageCode,
      field_Width
    };
    double getAspectRatio() const;
    const wstring& getFilePath() const;
    uint32_t getHeight() const;
    const wstring& getLanguageCode() const;
    uint32_t getWidth() const;
  };

  // Movie entry
  // http://help.themoviedb.org/kb/api/movie-info
  class Movie: public FieldBasedClass<22> {
  friend class TMDb;
  protected:
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
    enum FieldBits: FieldIndex {
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

  // Cast credit entry
  // http://help.themoviedb.org/kb/api/person-credits
  // http://help.themoviedb.org/kb/api/movie-casts
  struct CastCredit: public FieldBasedClass<1> {
  friend class TMDb;
  protected:
    struct Fields {
      wstring character;
    } mFields;
    Movie mMovie;
  public:
    enum FieldBits: FieldIndex {
      field_Character = 0
    };
    const wstring& getCharacter() const;
    const Movie& getMovie() const;
  };

  typedef list<CastCredit> CastCreditList;

  // Crew credit entry
  // http://help.themoviedb.org/kb/api/person-credits
  // http://help.themoviedb.org/kb/api/movie-casts
  class CrewCredit: public FieldBasedClass<2> {
  friend class TMDb;
  protected:
    struct Fields {
      wstring department;
      wstring job;
    } mFields;
    Movie mMovie;
  public:
    enum FieldBits: FieldIndex {
      field_Department = 0,
      field_Job
    };
    const wstring& getDepartment() const;
    const wstring& getJob() const;
    const Movie& getMovie() const;
  };

  typedef list<CrewCredit> CrewCreditList;

  // Person credits container
  // http://help.themoviedb.org/kb/api/person-credits
  struct PersonCredits {
  public:
    CastCreditList castCredits;
    CrewCreditList crewCredits;
  };

  class Exception: public std::runtime_error {
  public:
    Exception( const std::string& msg );
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
    void readJSONPerson( const js::wValue& jsonPerson, Person& person );
    void readJSONCastCredit( const js::wValue& jsonCredit, CastCredit& credit );
    void readJSONCrewCredit( const js::wValue& jsonCredit, CrewCredit& credit );
    void readJSONImage( const js::wValue& jsonImage, Image& image );
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
    void readJSONPagedPersonResults( const js::wValue& jsonResults,
      PagedPersonResults& results );
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
    PagedMovieResults getCompanyMovies( const Company& company, uint32_t page = 1 );
    PagedMovieResults getGenreMovies( uint32_t genre, uint32_t page = 1 );
    PagedMovieResults getGenreMovies( const Genre& genre, uint32_t page = 1 );
    // Person
    PersonCredits getPersonCredits( uint32_t person );
    PersonCredits getPersonCredits( const Person& person );
    // Companies
    Company getCompany( uint32_t id );
    // Search
    PagedMovieResults searchMovies( const wstring& query, bool adult = true,
      uint32_t year = 0, uint32_t page = 1 );
    PagedCompanyResults searchCompanies( const wstring& query, uint32_t page = 1 );
    PagedPersonResults searchPeople( const wstring& query, uint32_t page = 1 );
    // Genres
    GenreMap getGenres();
  };

}