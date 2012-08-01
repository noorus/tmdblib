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

  wstring TMDb::makeURL( LPCWSTR format, StringMap* query, ... )
  {
    va_list va_alist;
    wstring str( 512, NULL );
    va_start( va_alist, format );
    va_arg( va_alist, int ); // Skip query arg
    _vsnwprintf_s( &str[0], 512, 511, format, va_alist );
    va_end( va_alist );

    wstring url( 512, NULL );
    swprintf_s( &url[0], 512, L"http://%s/3/%s?api_key=%s",
      mAPIHost.c_str(), str.c_str(), mAPIKey.c_str() );

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

  void TMDb::readJSONProductionCompany( const js::wValue& jsonCompany,
  Company& company )
  {
    const js::wObject& obj = jsonCompany.getObject();
    company.id = obj.find( L"id" )->second.getInt();
    company.name = obj.find( L"name" )->second.getString();
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
          movie.mFields.companies[company.id] = company;
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
        // round to one decimal
        movie.mFields.voteAverage = floorf( (float)value.getReal() * 10.0f + 0.5f ) / 10.0f;
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

  Movie TMDb::getMovie( unsigned int id )
  {
    Movie movie;
    js::wValue jsonMovie = mClient->request( makeURL( L"movie/%d", NULL, id ) );
    readJSONMovie( jsonMovie, movie );
    return movie;
  }

  TMDb::~TMDb()
  {
    if ( mClient )
      delete mClient;
    Poco::Net::uninitializeNetwork();
  }

}