/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestState.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTSTATE_HPP_
#define HTTPREQUESTSTATE_HPP_

#include <string>

namespace Httpico {

enum HttpResponseState {
	OK = 200, NOT_FOUND = 404, INTERNAL_SERVER_ERROR = 500, FORBIDDEN = 403
};

std::string stateValueToString(HttpResponseState state);
std::string stateToString(HttpResponseState state);

} //namespace

#endif 
