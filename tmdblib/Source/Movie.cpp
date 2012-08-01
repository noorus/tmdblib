#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define MOVIE_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw std::exception( "Field does not exist: ##x##" )

  Movie::Fields::Fields(): adult( false ), budget( 0 ), id( 0 ),
    popularity( 0.0f ), revenue( 0 ), runtime( 0 ), voteAverage( 0.0f ),
    voteCount( 0 ) {}

  Movie::Movie()
  {
    mFieldBits.reset();
  }

  bool Movie::hasField( FieldBits field )
  {
    return mFieldBits[field];
  }

  bool Movie::isAdult()
  {
    MOVIE_CHECK_FIELD( field_Adult );
    return mFields.adult;
  }

  const wstring& Movie::getBackdropPath()
  {
    MOVIE_CHECK_FIELD( field_Backdrop );
    return mFields.backdropPath;
  }

  uint32_t Movie::getBudget()
  {
    MOVIE_CHECK_FIELD( field_Budget );
    return mFields.budget;
  }

  const wstring& Movie::getHomepage()
  {
    MOVIE_CHECK_FIELD( field_Homepage );
    return mFields.homepage;
  }

  uint32_t Movie::getID()
  {
    MOVIE_CHECK_FIELD( field_ID );
    return mFields.id;
  }

  const wstring& Movie::getIMDbID()
  {
    MOVIE_CHECK_FIELD( field_IMDbID );
    return mFields.imdbID;
  }

  const wstring& Movie::getOriginalTitle()
  {
    MOVIE_CHECK_FIELD( field_OriginalTitle );
    return mFields.originalTitle;
  }

  const wstring& Movie::getOverview()
  {
    MOVIE_CHECK_FIELD( field_Overview );
    return mFields.overview;
  }

  float Movie::getPopularity()
  {
    MOVIE_CHECK_FIELD( field_Popularity );
    return mFields.popularity;
  }

  const wstring& Movie::getPosterPath()
  {
    MOVIE_CHECK_FIELD( field_Poster );
    return mFields.posterPath;
  }

  const date& Movie::getReleaseDate()
  {
    MOVIE_CHECK_FIELD( field_ReleaseDate );
    return mFields.releaseDate;
  }

  uint32_t Movie::getRevenue()
  {
    MOVIE_CHECK_FIELD( field_Revenue );
    return mFields.revenue;
  }

  uint32_t Movie::getRuntime()
  {
    MOVIE_CHECK_FIELD( field_Runtime );
    return mFields.runtime;
  }

  const wstring& Movie::getTagline()
  {
    MOVIE_CHECK_FIELD( field_Tagline );
    return mFields.tagline;
  }

  const wstring& Movie::getTitle()
  {
    MOVIE_CHECK_FIELD( field_Title );
    return mFields.title;
  }

  float Movie::getVoteAverage()
  {
    MOVIE_CHECK_FIELD( field_VoteAverage );
    return mFields.voteAverage;
  }

  uint32_t Movie::getVoteCount()
  {
    MOVIE_CHECK_FIELD( field_VoteCount );
    return mFields.voteCount;
  }

}