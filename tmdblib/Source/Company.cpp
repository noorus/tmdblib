#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define COMPANY_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw std::exception( "Field does not exist: ##x##" )

  Company::Company()
  {
    mFieldBits.reset();
  }

  bool Company::hasField( FieldBits field ) const
  {
    return mFieldBits[field];
  }

  const wstring& Company::getDescription() const
  {
    COMPANY_CHECK_FIELD( field_Description );
    return mFields.description;
  }

  const wstring& Company::getHeadquarters() const
  {
    COMPANY_CHECK_FIELD( field_Headquarters );
    return mFields.headquarters;
  }

  const wstring& Company::getHomepage() const
  {
    COMPANY_CHECK_FIELD( field_Homepage );
    return mFields.homepage;
  }

  uint32_t Company::getID() const
  {
    COMPANY_CHECK_FIELD( field_ID );
    return mFields.id;
  }

  const wstring& Company::getLogoPath() const
  {
    COMPANY_CHECK_FIELD( field_LogoPath );
    return mFields.logoPath;
  }

  const wstring& Company::getName() const
  {
    COMPANY_CHECK_FIELD( field_Name );
    return mFields.name;
  }

}