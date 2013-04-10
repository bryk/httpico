/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServer.hpp"
#include <iostream>
#include "Utils.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestProcessor.hpp"
#include "HttpServerConfiguration.hpp"
#include "HttpResponse.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include "Logger.hpp"
#include <cerrno>

namespace Httpico {

HttpServer::HttpServer(HttpServerConfiguration &conf) :
		configuration(conf) {
	//initialize signal handlers
	Utils::initSignalHandlers();
}

HttpServer::~HttpServer() {
	close(socketFd);
}

void HttpServer::start() {
	Logger l(configuration);
	Logger::getInstance().dbg("Server root: %s\n", configuration.getServerRoot().c_str());
	initialize();
	Logger::getInstance().log("Server started\n");
	while (!Utils::shouldExit()) {
		int connectionFd;
		if ((connectionFd = accept(socketFd, NULL, NULL)) < 0) {
			if (errno == EMFILE) {
				usleep(1000);
				continue;
			} else if (errno != EINTR) {
				Logger::getInstance().log("Error accepting connection: %s\n", sys_errlist[errno]);
			}
			continue;
		} else {
			HttpRequest *httpRequest = new HttpRequest(connectionFd);
			HttpResponse *httpResponse = new HttpResponse(connectionFd);
			HttpRequestProcessor *processor = new HttpRequestProcessor(httpRequest, httpResponse, configuration);
			processor->handleRequest();
		}
	}
	Utils::waitForAllThreadsToTerminate();
	Logger::getInstance().log("Closing server...\n");
}

void HttpServer::initialize() {
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1) {
		Utils::setShouldExit();
		Logger::getInstance().log("socket() failed: %s\n", sys_errlist[errno]);
		return;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr)); //zeroing
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(configuration.getServerPort());

	int reuse = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) < 0) {
		Utils::setShouldExit();
		Logger::getInstance().log("setsockopt() failed: %s\n", sys_errlist[errno]);
		return;
	}

	if (bind(socketFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		Utils::setShouldExit();
		Logger::getInstance().log("bind() error: %s\n", sys_errlist[errno]);
		return;
	}

	if (listen(socketFd, 1024) < 0) { //TODO unhardcode
		Utils::setShouldExit();
		Logger::getInstance().log("listen() error: %s\n", sys_errlist[errno]);
		return;
	}
}

} //namespace
