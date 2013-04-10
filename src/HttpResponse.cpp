/**
 =========================================================================================
 File name	: /httpico/include/HttpResponse.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpResponse.hpp"
#include "unistd.h"
#include <sys/socket.h>
#include "HttpResponseState.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

namespace Httpico {

HttpResponse::HttpResponse(int sockketFd) :
		state(INTERNAL_SERVER_ERROR), bytesTransferred(0), socketFd_(sockketFd) {

}

HttpResponse::~HttpResponse() {
	shutdown(socketFd_, 1);
	close(socketFd_);
}

void HttpResponse::writeResponse(const Buffer &buf) {
	if (write(socketFd_, buf.data(), sizeof(char) * buf.size()) == -1) {
		Logger::getInstance().dbg("Couldn't send data to the client\n");
	}
	bytesTransferred += buf.size();
}

}
