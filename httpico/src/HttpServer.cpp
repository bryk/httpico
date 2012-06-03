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
	Logger::getInstance().log("Niszczę serwer\n");
	close(socketFd);
}

void HttpServer::start() {
	Logger l(configuration);
	Logger::getInstance().log("Serwer startuje\n");
	Logger::getInstance().log("Server root: %s\n", configuration.getServerRoot().c_str());
	initialize();
	while (!Utils::shouldExit()) {
		//acceptNewSocket();
		int connectionFd;
		if ((connectionFd = accept(socketFd, NULL, NULL)) < 0) {
			if (errno != EINTR) {
				Logger::getInstance().dbg("Błąd akceptowania\n");
				perror(NULL);
			}
			continue;
		} else {
			Logger::getInstance().log("Zaakceptowano połączenie!!!\n");
			HttpRequest *httpRequest = new HttpRequest(connectionFd);
			HttpResponse *httpResponse = new HttpResponse(connectionFd);
			HttpRequestProcessor processor(httpRequest, httpResponse, configuration);
			processor.process();
		}
	}
	Logger::getInstance().log("Zamykam serwer\n");
}

void HttpServer::initialize() {
	Logger::getInstance().dbg("Zaczynam inicjalizować\n");
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1) {
		Utils::setShouldExit();
		Logger::getInstance().dbg("Krytyczny błąd\n");
		perror(NULL);
		return;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr)); //zerujemy
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(configuration.getServerPort());

	int reuse = 1;
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) < 0) {
		Utils::setShouldExit();
		Logger::getInstance().dbg("setsockopt() failed\n");
		perror(NULL);
		return;
	}

	if (bind(socketFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		Utils::setShouldExit();
		Logger::getInstance().dbg("Krytyczny błąd bind\n");
		perror(NULL);
		return;
	}

	if (listen(socketFd, 1024) < 0) { //TODO unhardcode
		Utils::setShouldExit();
		Logger::getInstance().dbg("Krytyczny błąd listen\n");
		perror(NULL);
		return;
	}

	Logger::getInstance().dbg("Skończyłem inicjalizować\n");
}

} //namespace
