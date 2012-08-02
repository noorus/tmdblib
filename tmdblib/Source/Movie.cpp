#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define MOVIE_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw std::exception( "Field does not exist: ##x##" )

  Movie::Movie()
  {
    mFieldBits.reset();
  }

  bool Movie::hasField( FieldBits field ) const
  {
    return mFieldBits[field];
  }

  bool Movie::isAdult() const
  {
    MOVIE_CHECK_FIELD( field_Adult );
    return mFields.adult;
  }

  const wstring& Movie::getBackdropPath() const
  {
    MOVIE_CHECK_FIELD( field_Backdrop );
    return mFields.backdropPath;
  }

  uint32_t Movie::getBudget() const
  {
    MOVIE_CHECK_FIELD( field_Budget );
    return mFields.budget;
  }

  const CollectionMap& Movie::getCollections() const
  {
    MOVIE_CHECK_FIELD( field_Collections );
    return mFields.collections;
  }

  const GenreMap& Movie::getGenres() const
  {
    MOVIE_CHECK_FIELD( field_Genres );
    return mFields.genres;
  }

  const wstring& Movie::getHomepage() const
  {
    MOVIE_CHECK_FIELD( field_Homepage );
    return mFields.homepage;
  }

  uint32_t Movie::getID() const
  {
    MOVIE_CHECK_FIELD( field_ID );
    return mFields.id;
  }

  const wstring& Movie::getIMDbID() const
  {
    MOVIE_CHECK_FIELD( field_IMDbID );
    return mFields.imdbID;
  }

  const wstring& Movie::getOriginalTitle() const
  {
    MOVIE_CHECK_FIELD( field_OriginalTitle );
    return mFields.originalTitle;
  }

  const wstring& Movie::getOverview() const
  {
    MOVIE_CHECK_FIELD( field_Overview );
    return mFields.overview;
  }

  double Movie::getPopularity() const
  {
    MOVIE_CHECK_FIELD( field_Popularity );
    return mFields.popularity;
  }

  const wstring& Movie::getPosterPath() const
  {
    MOVIE_CHECK_FIELD( field_Poster );
    return mFields.posterPath;
  }

  const CompanyMap& Movie::getProductionCompanies() const
  {
    MOVIE_CHECK_FIELD( field_ProductionCompanies );
    return mFields.companies;
  }

  const CountryMap& Movie::getProductionCountries() const
  {
    MOVIE_CHECK_FIELD( field_ProductionCountries );
    return mFields.countries;
  }

  const date& Movie::getReleaseDate() const
  {
    MOVIE_CHECK_FIELD( field_ReleaseDate );
    return mFields.releaseDate;
  }

  uint32_t Movie::getRevenue() const
  {
    MOVIE_CHECK_FIELD( field_Revenue );
    return mFields.revenue;
  }

  uint32_t Movie::getRuntime() const
  {
    MOVIE_CHECK_FIELD( field_Runtime );
    return mFields.runtime;
  }

  const LanguageMap& Movie::getSpokenLanguages() const
  {
    MOVIE_CHECK_FIELD( field_SpokenLanguages );
    return mFields.languages;
  }

  const wstring& Movie::getTagline() const
  {
    MOVIE_CHECK_FIELD( field_Tagline );
    return mFields.tagline;
  }

  const wstring& Movie::getTitle() const
  {
    MOVIE_CHECK_FIELD( field_Title );
    return mFields.title;
  }

  float Movie::getVoteAverage() const
  {
    MOVIE_CHECK_FIELD( field_VoteAverage );
    return mFields.voteAverage;
  }

  uint32_t Movie::getVoteCount() const
  {
    MOVIE_CHECK_FIELD( field_VoteCount );
    return mFields.voteCount;
  }

}