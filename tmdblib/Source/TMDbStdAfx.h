#pragma once

#include "TMDbTargetVer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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