#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

# define IMAGE_CHECK_FIELD(x) if ( !mFieldBits[##x##] )\
  throw Exception( "Field does not exist in image: " #x )

  double Image::getAspectRatio() const
  {
    IMAGE_CHECK_FIELD( field_AspectRatio );
    return mFields.aspectRatio;
  }

  const wstring& Image::getFilePath() const
  {
    IMAGE_CHECK_FIELD( field_FilePath );
    return mFields.filePath;
  }

  uint32_t Image::getHeight() const
  {
    IMAGE_CHECK_FIELD( field_Height );
    return mFields.height;
  }

  const wstring& Image::getLanguageCode() const
  {
    IMAGE_CHECK_FIELD( field_LanguageCode );
    return mFields.languageCode;
  }

  uint32_t Image::getWidth() const
  {
    IMAGE_CHECK_FIELD( field_Width );
    return mFields.width;
  }


}