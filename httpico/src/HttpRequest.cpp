/**
 =========================================================================================
 File name	: /httpico/src/HttpRequest.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequest.hpp"
#include "Utils.hpp"
#include <sys/socket.h>
#include <unistd.h>

namespace Httpico {

HttpRequest::HttpRequest(int socketFd) :
		socketFd_(socketFd) {
}

HttpRequest::~HttpRequest() {
	close(socketFd_);
}

} //namespace

