/**
 =========================================================================================
 File name	: /httpico/include/HttpResponse.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpResponse.hpp"
#include "unistd.h"
#include <sys/socket.h>
#include "Utils.hpp"

namespace Httpico {

HttpResponse::HttpResponse(int sockketFd) :
		socketFd_(sockketFd) {

}

HttpResponse::~HttpResponse() {
	shutdown(socketFd_, 1);
}

void HttpResponse::writeResponse(Buffer &buf) {
	if (write(socketFd_, buf.c_str(), sizeof(char) * buf.size()) == -1) {
		Utils::dbg("Nie udało się odpowiedzieć klientowi\n");
	}
}

}
