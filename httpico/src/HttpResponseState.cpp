/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestState.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include <string>

#include "HttpResponseState.hpp"
#include <sstream>

namespace Httpico {

std::string stateToString(HttpResponseState state) {
	switch (state) {
	case OK:
		return "Ok";
		break;
	case NOT_FOUND:
		return "Not Found";
		break;
	case FORBIDDEN:
		return "Forbidden";
		break;
	case INTERNAL_SERVER_ERROR:
		return "Internal Server Error";
		break;
	}
	return "";
}

std::string stateValueToString(HttpResponseState state) {
	std::ostringstream out;
	out << state;
	return out.str();
}

} //namespace
