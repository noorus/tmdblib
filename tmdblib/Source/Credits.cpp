#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define CREDITS_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw Exception( "Field does not exist in credit: " #x )

  const wstring& CastCredit::getCharacter() const
  {
    CREDITS_CHECK_FIELD( field_Character );
    return mFields.character;
  }

  const Movie& CastCredit::getMovie() const
  {
    return mMovie;
  }

  const wstring& CrewCredit::getDepartment() const
  {
    CREDITS_CHECK_FIELD( field_Department );
    return mFields.department;
  }

  const wstring& CrewCredit::getJob() const
  {
    CREDITS_CHECK_FIELD( field_Job );
    return mFields.job;
  }

  const Movie& CrewCredit::getMovie() const
  {
    return mMovie;
  }

}