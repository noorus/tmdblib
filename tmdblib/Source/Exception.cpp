#include "TMDbStdAfx.h"
#include "TMDb.h"
#include "TMDbUtilities.h"

namespace TMDb {

  Exception::Exception( const string& msg ): std::runtime_error( msg )
  {
    // Stubb
  }

}