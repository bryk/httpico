/**
 =========================================================================================
 File name	: /httpico/src/HttpRequest.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequest.hpp"
#include "HttpRequestState.hpp"
#include <iostream>

namespace Httpico {

HttpRequest::HttpRequest(socketPtr sock_) :
		state(JUST_CREATED), sock(sock_) {
	std::cout << "HttpRequest" << std::endl;
}

HttpRequest::~HttpRequest() {
	// TODO Auto-generated destructor stub
}

} //namespace

