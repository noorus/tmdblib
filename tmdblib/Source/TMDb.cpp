#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

  const wstring TMDb::mAPIHost( L"api.themoviedb.org" );

  TMDb::TMDb( const std::wstring& apiKey ): mAPIKey( apiKey ), mClient( NULL )
  {
    Poco::Net::initializeNetwork();
    mClient = new JSONClient( mAPIHost, 80 );
    fetchConfiguration();
  }

  wstring TMDb::makeURL( const wstring& method, StringMap* query )
  {
    wstring url( 512, NULL );
    int len = swprintf_s( &url[0], 512, L"http://%s/3/%s?api_key=%s",
      mAPIHost.c_str(), method.c_str(), mAPIKey.c_str() );
    url.resize( len );
    if ( query )
      for ( StringMap::const_iterator it = query->begin(); it != query->end(); ++it )
      {
        WCHAR qstr[512];
        swprintf_s( qstr, 512, L"&%s=%s", (*it).first.c_str(), (*it).second.c_str() );
        url.append( qstr );
      }
    return url;
  }

  void TMDb::readJSONArray( const json_spirit::wValue& val,
  const wstring& path, StringVector& vec )
  {
    vec.clear();
    js::wArray arr = val.getArray( path );
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
      vec.push_back( (*it).getString() );
  }

  void TMDb::readJSONGenre( const js::wValue& jsonGenre, Genre& genre )
  {
    const js::wObject& obj = jsonGenre.getObject();
    genre.id = obj.find( L"id" )->second.getInt();
    genre.name = obj.find( L"name" )->second.getString();
  }

  void TMDb::readJSONPerson( const js::wValue& jsonPerson, Person& person )
  {
    const js::wObject& obj = jsonPerson.getObject();
    for ( js::wObject::const_iterator it = obj.begin(); it != obj.end(); ++it )
    {
      const wstring& key = it->first;
      const js::wValue& value = it->second;
      
      if ( value.isNull() )
        continue;

      if ( key == L"adult" ) {
        person.mFields.adult = value.getBool();
        person.mFieldBits[Person::field_Adult] = true;
      } else if ( key == L"also_known_as" ) {
        // Todo
      } else if ( key == L"biography" ) {
        person.mFields.biography = value.getString();
        person.mFieldBits[Person::field_Biography] = true;
      } else if ( key == L"birthday" ) {
        person.mFields.birthday = boost::gregorian::from_simple_string(
          wideToString( value.getString() ) );
        person.mFieldBits[Person::field_Birthday] = true;
      } else if ( key == L"deathday" ) {
        person.mFields.deathday = boost::gregorian::from_simple_string(
          wideToString( value.getString() ) );
        person.mFieldBits[Person::field_Deathday] = true;
      } else if ( key == L"homepage" ) {
        person.mFields.homepage = value.getString();
        person.mFieldBits[Person::field_Homepage] = true;
      } else if ( key == L"id" ) {
        person.mFields.id = value.getInt();
        person.mFieldBits[Person::field_ID] = true;
      } else if ( key == L"name" ) {
        person.mFields.name = value.getString();
        person.mFieldBits[Person::field_Name] = true;
      } else if ( key == L"place_of_birth" ) {
        person.mFields.placeOfBirth = value.getString();
        person.mFieldBits[Person::field_PlaceOfBirth] = true;
      } else if ( key == L"profile_path" ) {
        person.mFields.profilePath = value.getString();
        person.mFieldBits[Person::field_ProfilePath] = true;
      }
    }
  }

  void TMDb::readJSONProductionCompany( const js::wValue& jsonCompany,
  Company& company )
  {
    const js::wObject& obj = jsonCompany.getObject();
    for ( js::wObject::const_iterator it = obj.begin(); it != obj.end(); ++it )
    {
      const wstring& key = it->first;
      const js::wValue& value = it->second;

      if ( value.isNull() )
        continue;

      if ( key == L"description" ) {
        company.mFields.description = value.getString();
        company.mFieldBits[Company::field_Description] = true;
      } else if ( key == L"headquarters" ) {
        company.mFields.headquarters = value.getString();
        company.mFieldBits[Company::field_Headquarters] = true;
      } else if ( key == L"homepage" ) {
        company.mFields.homepage = value.getString();
        company.mFieldBits[Company::field_Homepage] = true;
      } else if ( key == L"id" ) {
        company.mFields.id = value.getInt();
        company.mFieldBits[Company::field_ID] = true;
      } else if ( key == L"logo_path" ) {
        company.mFields.logoPath = value.getString();
        company.mFieldBits[Company::field_LogoPath] = true;
      } else if ( key == L"name" ) {
        company.mFields.name = value.getString();
        company.mFieldBits[Company::field_Name] = true;
      }
    }
  }

  void TMDb::readJSONProductionCountry( const js::wValue& jsonCountry,
  Country& country )
  {
    const js::wObject& obj = jsonCountry.getObject();
    country.code = obj.find( L"iso_3166_1" )->second.getString();
    country.name = obj.find( L"name" )->second.getString();
  }

  void TMDb::readJSONSpokenLanguage( const js::wValue& jsonLanguage,
  Language& language )
  {
    const js::wObject& obj = jsonLanguage.getObject();
    language.code = obj.find( L"iso_639_1" )->second.getString();
    language.name = obj.find( L"name" )->second.getString();
  }

  void TMDb::readJSONCollection( const js::wValue& jsonCollection,
  Collection& collection )
  {
    const js::wObject& obj = jsonCollection.getObject();
    for ( js::wObject::const_iterator it = obj.begin(); it != obj.end(); ++it )
    {
      const wstring& key = it->first;
      const js::wValue& value = it->second;

      if ( value.isNull() )
        continue;

      if ( key == L"backdrop_path" ) {
        collection.backdropPath = value.getString();
      } else if ( key == L"id" ) {
        collection.id = value.getInt();
      } else if ( key == L"name" ) {
        collection.name = value.getString();
      } else if ( key == L"poster_path" ) {
        collection.posterPath = value.getString();
      }
    }
  }

  void TMDb::readJSONPagedMovieResults( const js::wValue& jsonResults,
  PagedMovieResults& results )
  {
    const js::wObject& obj = jsonResults.getObject();
    results.page = obj.find( L"page" )->second.getInt();
    js::wArray arr = obj.find( L"results" )->second.getArray();
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
    {
      Movie movie;
      readJSONMovie( (*it), movie );
      results.results.push_back( movie );
    }
    results.totalPages = obj.find( L"total_pages" )->second.getInt();
    results.totalResults = obj.find( L"total_results" )->second.getInt();
  }

  void TMDb::readJSONPagedCompanyResults( const js::wValue& jsonResults,
  PagedCompanyResults& results )
  {
    const js::wObject& obj = jsonResults.getObject();
    results.page = obj.find( L"page" )->second.getInt();
    js::wArray arr = obj.find( L"results" )->second.getArray();
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
    {
      Company company;
      readJSONProductionCompany( (*it), company );
      results.results.push_back( company );
    }
    results.totalPages = obj.find( L"total_pages" )->second.getInt();
    results.totalResults = obj.find( L"total_results" )->second.getInt();
  }

  void TMDb::readJSONPagedPersonResults( const js::wValue& jsonResults,
  PagedPersonResults& results )
  {
    const js::wObject& obj = jsonResults.getObject();
    results.page = obj.find( L"page" )->second.getInt();
    js::wArray arr = obj.find( L"results" )->second.getArray();
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
    {
      Person person;
      readJSONPerson( (*it), person );
      results.results.push_back( person );
    }
    results.totalPages = obj.find( L"total_pages" )->second.getInt();
    results.totalResults = obj.find( L"total_results" )->second.getInt();
  }

  void TMDb::readJSONMovie( const js::wValue& jsonMovie, Movie& movie )
  {
    const js::wObject& obj = jsonMovie.getObject();
    for ( js::wObject::const_iterator it = obj.begin(); it != obj.end(); ++it )
    {
      const wstring& key = it->first;
      const js::wValue& value = it->second;

      if ( value.isNull() )
        continue;

      if ( key == L"adult" ) {
        movie.mFields.adult = value.getBool();
        movie.mFieldBits[Movie::field_Adult] = true;
      } else if ( key == L"backdrop_path" ) {
        movie.mFields.backdropPath = value.getString();
        movie.mFieldBits[Movie::field_Backdrop] = true;
      } else if ( key == L"belongs_to_collection" ) {
        Collection collection;
        readJSONCollection( value, collection );
        movie.mFields.collections[collection.id] = collection;
        movie.mFieldBits[Movie::field_Collections] = true;
      } else if ( key == L"budget" ) {
        movie.mFields.budget = value.getInt();
        movie.mFieldBits[Movie::field_Budget] = true;
      } else if ( key == L"genres" ) {
        js::wArray arr = value.getArray();
        for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
        {
          Genre genre;
          readJSONGenre( (*it), genre );
          movie.mFields.genres[genre.id] = genre;
        }
        movie.mFieldBits[Movie::field_Genres] = true;
      } else if ( key == L"homepage" ) {
        movie.mFields.homepage = value.getString();
        movie.mFieldBits[Movie::field_Homepage] = true;
      } else if ( key == L"id" ) {
        movie.mFields.id = value.getInt();
        movie.mFieldBits[Movie::field_ID] = true;
      } else if ( key == L"imdb_id" ) {
        movie.mFields.imdbID = value.getString();
        movie.mFieldBits[Movie::field_IMDbID] = true;
      } else if ( key == L"original_title" ) {
        movie.mFields.originalTitle = value.getString();
        movie.mFieldBits[Movie::field_OriginalTitle] = true;
      } else if ( key == L"overview" ) {
        movie.mFields.overview = value.getString();
        movie.mFieldBits[Movie::field_Overview] = true;
      } else if ( key == L"popularity" ) {
        movie.mFields.popularity = value.getReal();
        movie.mFieldBits[Movie::field_Popularity] = true;
      } else if ( key == L"poster_path" ) {
        movie.mFields.posterPath = value.getString();
        movie.mFieldBits[Movie::field_Poster] = true;
      } else if ( key == L"production_companies" ) {
        js::wArray arr = value.getArray();
        for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
        {
          Company company;
          readJSONProductionCompany( (*it), company );
          movie.mFields.companies[company.getID()] = company;
        }
        movie.mFieldBits[Movie::field_ProductionCompanies] = true;
      } else if ( key == L"production_countries" ) {
        js::wArray arr = value.getArray();
        for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
        {
          Country country;
          readJSONProductionCountry( (*it), country );
          movie.mFields.countries[country.code] = country;
        }
        movie.mFieldBits[Movie::field_ProductionCountries] = true;
      } else if ( key == L"release_date" ) {
        movie.mFields.releaseDate = boost::gregorian::from_simple_string(
          wideToString( value.getString() ) );
        movie.mFieldBits[Movie::field_ReleaseDate] = true;
      } else if ( key == L"revenue" ) {
        movie.mFields.revenue = value.getInt();
        movie.mFieldBits[Movie::field_Revenue] = true;
      } else if ( key == L"runtime" ) {
        movie.mFields.runtime = value.getInt();
        movie.mFieldBits[Movie::field_Runtime] = true;
      } else if ( key == L"spoken_languages" ) {
        js::wArray arr = value.getArray();
        for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
        {
          Language language;
          readJSONSpokenLanguage( (*it), language );
          movie.mFields.languages[language.code] = language;
        }
        movie.mFieldBits[Movie::field_SpokenLanguages] = true;
      } else if ( key == L"tagline" ) {
        movie.mFields.tagline = value.getString();
        movie.mFieldBits[Movie::field_Tagline] = true;
      } else if ( key == L"title" ) {
        movie.mFields.title = value.getString();
        movie.mFieldBits[Movie::field_Title] = true;
      } else if ( key == L"vote_average" ) {
        movie.mFields.voteAverage = (float)value.getReal();
        movie.mFieldBits[Movie::field_VoteAverage] = true;
      } else if ( key == L"vote_count" ) {
        movie.mFields.voteCount = value.getInt();
        movie.mFieldBits[Movie::field_VoteCount] = true;
      }
    }
  }

  void TMDb::fetchConfiguration()
  {
    js::wValue val = mClient->request( makeURL( L"configuration" ) );

    mConfiguration.baseURL = val.getString( L"images.base_url" );

    readJSONArray( val, L"images.poster_sizes", mConfiguration.posterSizes );
    readJSONArray( val, L"images.backdrop_sizes", mConfiguration.backdropSizes );
    readJSONArray( val, L"images.profile_sizes", mConfiguration.profileSizes );
    readJSONArray( val, L"images.logo_sizes", mConfiguration.logoSizes );
  }

  const Configuration& TMDb::getConfiguration()
  {
    return mConfiguration;
  }

  Movie TMDb::getMovie( uint32_t id )
  {
    Movie movie;
    js::wValue jsonMovie = mClient->request( makeURL(
      widePrintf( L"movie/%d", id ) ) );
    readJSONMovie( jsonMovie, movie );
    return movie;
  }

  Movie TMDb::getLatestMovie()
  {
    Movie movie;
    js::wValue jsonMovie = mClient->request( makeURL( L"movie/latest" ) );
    readJSONMovie( jsonMovie, movie );
    return movie;
  }

  PagedMovieResults TMDb::getUpcomingMovies( uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"movie/upcoming", &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results;
  }

  PagedMovieResults TMDb::getNowPlayingMovies( uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"movie/now_playing", &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results;
  }

  PagedMovieResults TMDb::getPopularMovies( uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"movie/popular", &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results;
  }

  PagedMovieResults TMDb::getTopRatedMovies( uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"movie/top_rated", &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results;
  }

  PagedMovieResults TMDb::getCompanyMovies( uint32_t company, uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      widePrintf( L"company/%d/movies", company ), &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results; 
  }

  PagedMovieResults TMDb::getGenreMovies( uint32_t genre, uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      widePrintf( L"genre/%d/movies", genre ), &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results; 
  }

  PagedMovieResults TMDb::getGenreMovies( Genre genre, uint32_t page )
  {
    return getGenreMovies( genre.id, page );
  }

  PagedMovieResults TMDb::searchMovies( const wstring& _query,
  bool adult, uint32_t year, uint32_t page )
  {
    PagedMovieResults results;
    StringMap query;
    query[L"query"] = _query;
    query[L"include_adult"] = adult ? L"true" : L"false";
    if ( year > 0 )
      query[L"year"] = static_cast<wstringstream const&>(
      wstringstream() << year ).str();
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"search/movie", &query ) );
    readJSONPagedMovieResults( jsonResults, results );
    return results;
  }

  PagedCompanyResults TMDb::searchCompanies( const wstring& _query,
  uint32_t page )
  {
    PagedCompanyResults results;
    StringMap query;
    query[L"query"] = _query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"search/company", &query ) );
    readJSONPagedCompanyResults( jsonResults, results );
    return results;
  }

  PagedPersonResults TMDb::searchPeople( const wstring& _query,
  uint32_t page )
  {
    PagedPersonResults results;
    StringMap query;
    query[L"query"] = _query;
    if ( page > 1 )
      query[L"page"] = static_cast<wstringstream const&>(
      wstringstream() << page ).str();
    js::wValue jsonResults = mClient->request( makeURL(
      L"search/person", &query ) );
    readJSONPagedPersonResults( jsonResults, results );
    return results;
  }

  Company TMDb::getCompany( uint32_t id )
  {
    Company company;
    js::wValue jsonCompany = mClient->request( makeURL(
      widePrintf( L"company/%d", id ) ) );
    readJSONProductionCompany( jsonCompany, company );
    return company;
  }

  GenreMap TMDb::getGenres()
  {
    GenreMap genres;
    js::wValue jsonGenres = mClient->request( makeURL( L"genre/list" ) );
    js::wArray arr = jsonGenres.getArray( L"genres" );
    for ( js::wArray::iterator it = arr.begin(); it != arr.end(); ++it )
    {
      Genre genre;
      readJSONGenre( (*it), genre );
      genres[genre.id] = genre;
    }
    return genres;
  }

  TMDb::~TMDb()
  {
    if ( mClient )
      delete mClient;
    Poco::Net::uninitializeNetwork();
  }

}