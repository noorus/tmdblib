#pragma once

#include "TargetVer.h"

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
#include <Poco/Net/Net.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPClientSession.h>