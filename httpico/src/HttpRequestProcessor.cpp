/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor.hpp"
#include "Utils.hpp"

#include <sys/socket.h>
#include "HttpRequest.hpp"
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <string.h>

namespace Httpico {

HttpRequestProcessor::HttpRequestProcessor(HttpRequest * httpRequest) :
		httpRequest_(httpRequest) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
	Utils::dbg("Niszcze processora\n");
	delete httpRequest_;
}

void HttpRequestProcessor::process() {
	Utils::dbg("Processuje requesta\n");
	char buf[10000];
	while (true) {
		int sz = recv(httpRequest_->getSocketFd(), buf, sizeof(buf), 0);
		if (sz > 0) {
			buf[sz] = 0;
			Utils::dbg("Wczytano: '%s'\n", buf);
			if (buf[sz - 1] == '\n') {
				//end;
				if (write(httpRequest_->getSocketFd(), buf, sizeof(char) * strlen(buf)) == -1) {
					Utils::dbg("Nie udało się odpowiedzieć klientowi");
				}
				break;
			}
		} else {
			Utils::dbg("Koniec: %d\n", sz);
			perror(NULL);
			break;
		}
	}
}

}
