/**
 =========================================================================================
 File name	: /httpico/src/HttpServerConfiguration.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServerConfiguration.hpp"

namespace Httpico {

HttpServerConfiguration::HttpServerConfiguration() :
		serverPort(DEFAULT_SERVER_PORT), requestBufferSize(REQUEST_BUFFER_SIZE), serverRoot(getcwd(NULL, 0)) {

}

HttpServerConfiguration::~HttpServerConfiguration() {
}

const std::string &HttpServerConfiguration::getServerRoot() {
	return serverRoot;
}

void HttpServerConfiguration::setServerRoot(const std::string &srv) {
	serverRoot = srv;
}

int HttpServerConfiguration::getServerPort() {
	return serverPort;
}

void HttpServerConfiguration::setServerPort(int p) {
	serverPort = p;
}

int HttpServerConfiguration::getRequestBufferSize() {
	return requestBufferSize;
}

void HttpServerConfiguration::setRequestBufferSize(int p) {
	requestBufferSize = p;
}

}
