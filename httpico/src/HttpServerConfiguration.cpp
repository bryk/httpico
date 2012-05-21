/**
 =========================================================================================
 File name	: /httpico/src/HttpServerConfiguration.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServerConfiguration.hpp"
#include "config.hpp"
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include "Utils.hpp"

namespace Httpico {

#ifndef EMBEDDED
#define DEFAULT_SERVER_ROOT "/usr/share/httpico/srv"
#define DEFAULT_SERVER_TEMPLATE_ROOT "/usr/share/httpico/"
#else
#define DEFAULT_SERVER_ROOT "./"
#define DEFAULT_SERVER_TEMPLATE_ROOT "./"
#endif

HttpServerConfiguration::HttpServerConfiguration() :
		serverPort(DEFAULT_SERVER_PORT), requestBufferSize(REQUEST_BUFFER_SIZE), serverRoot(DEFAULT_SERVER_ROOT), serverTemplateRoot(
				DEFAULT_SERVER_TEMPLATE_ROOT) {
	char *rpathBuf;
	if ((rpathBuf = realpath(serverRoot.c_str(), NULL)) == NULL) { //let's change to real path
		perror(serverRoot.c_str());
		exit(EXIT_FAILURE);
	} else {
		serverRoot = rpathBuf;
	}
	free(rpathBuf);

	if ((rpathBuf = realpath(serverTemplateRoot.c_str(), NULL)) == NULL) { //let's change to real path
		perror(serverTemplateRoot.c_str());
		exit(EXIT_FAILURE);
	} else {
		serverTemplateRoot = rpathBuf;
	}
	free(rpathBuf);
}

HttpServerConfiguration::~HttpServerConfiguration() {
}

const std::string &HttpServerConfiguration::getServerTemplateRoot() {
	return serverTemplateRoot;
}

void HttpServerConfiguration::setServerTemplateRoot(const std::string &srv) {
	serverTemplateRoot = srv;
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
