#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define PERSON_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw Exception( "Field does not exist in person: " #x )

  Person::Person()
  {
    mFieldBits.reset();
  }

  bool Person::hasField( FieldBits field ) const
  {
    return mFieldBits[field];
  }

  bool Person::isAdult() const
  {
    PERSON_CHECK_FIELD( field_Adult );
    return mFields.adult;
  }

  const wstring& Person::getBiography() const
  {
    PERSON_CHECK_FIELD( field_Biography );
    return mFields.biography;
  }

  const date& Person::getBirthday() const
  {
    PERSON_CHECK_FIELD( field_Birthday );
    return mFields.birthday;
  }

  const date& Person::getDeathday() const
  {
    PERSON_CHECK_FIELD( field_Deathday );
    return mFields.deathday;
  }

  const wstring& Person::getHomepage() const
  {
    PERSON_CHECK_FIELD( field_Homepage );
    return mFields.homepage;
  }

  uint32_t Person::getID() const
  {
    PERSON_CHECK_FIELD( field_ID );
    return mFields.id;
  }

  const wstring& Person::getName() const
  {
    PERSON_CHECK_FIELD( field_Name );
    return mFields.name;
  }

  const wstring& Person::getPlaceOfBirth() const
  {
    PERSON_CHECK_FIELD( field_PlaceOfBirth );
    return mFields.placeOfBirth;
  }

  const wstring& Person::getProfilePath() const
  {
    PERSON_CHECK_FIELD( field_ProfilePath );
    return mFields.profilePath;
  }


}