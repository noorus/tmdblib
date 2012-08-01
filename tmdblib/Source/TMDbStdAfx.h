#pragma once

#include "TMDbTargetVer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <exception>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <regex>
#include <stack>

#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPClientSession.h>

#define JSON_SPIRIT_VALUE_ENABLED
#include "../json_spirit/reader_template.h"
#include "../json_spirit/writer_template.h"