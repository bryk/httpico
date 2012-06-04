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
#include <sys/socket.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Logger.hpp"

namespace Httpico {

namespace {
bool endOfRequest(Buffer &buf) {
	std::string endIndicator = "\r\n\r\n";
	std::string lengthIndicator = "Content-Length:";

	size_t headerEnd = buf.find(endIndicator);

	if (headerEnd != std::string::npos) {
		size_t clenght = 0;
		size_t clengthPos = buf.find(lengthIndicator);
		if (clengthPos != std::string::npos) {
			size_t pos = clengthPos + lengthIndicator.size();
			std::string numstr = buf.substr(pos, 10);
			std::istringstream numRd(numstr);
			numRd >> clenght;
		}
		std::string rest = buf.substr(headerEnd + endIndicator.size());
		if (rest.size() == clenght) {
			return true;
		} else {
			return false;
		}
	} else { //didn't find end-of-requests
		return false;
	}
}
}

const std::pair<std::string, std::string> HttpRequest::getIthHeader(size_t i) {
	std::map<std::string, std::string>::iterator it = header.begin();
	while (i--) {
		it++;
	}
	return *it;
}

const size_t HttpRequest::getNumOfHeaders() {
	return header.size();
}

const std::pair<std::string, std::string> HttpRequest::getIthGetArg(size_t i) {
	std::map<std::string, std::string>::iterator it = getArgs.begin();
	while (i--) {
		it++;
	}
	return *it;
}

const size_t HttpRequest::getNumOfGetArgs() {
	return getArgs.size();
}

void HttpRequest::setGetArg(const std::string &key, const std::string &value) {
	getArgs[key] = value;
}

const std::string &HttpRequest::getGetArg(const std::string &key) {
	return getArgs[key];
}

void HttpRequest::setHeader(const std::string &key, const std::string &value) {
	header[key] = value;
}

const std::string &HttpRequest::getHeader(const std::string &key) {
	return header[key];
}

std::string HttpRequest::requestTypeToString(RequestType req) {
	if (req == GET) {
		return "GET";
	} else if (req == POST) {
		return "POST";
	} else if (req == UNKNOWN) {
		return "UNKNOWN";
	} else {
		return "UNKNOWN";
	}
}

HttpRequest::HttpRequest(int socketFd) :
		requestType(UNKNOWN), socketFd_(socketFd) {

	struct sockaddr_in m_addr;
	socklen_t len = sizeof m_addr;
	getpeername(socketFd_, (struct sockaddr*) &m_addr, &len);
	char buf[INET_ADDRSTRLEN];
	const char *ret = inet_ntop(AF_INET, &m_addr.sin_addr, buf, INET_ADDRSTRLEN);
	if (ret != NULL) {
		clientAddress = buf;
	}
}

HttpRequest::~HttpRequest() {
	shutdown(socketFd_, 0);
}

Buffer & HttpRequest::readRequest() {
	Buffer *buffer = new Buffer();

	char recvBuf[1000];
	while (true) {
		int sz = recv(socketFd_, recvBuf, sizeof(recvBuf), 0);
		if (sz > 0) {
			recvBuf[sz] = 0;
			buffer->append(recvBuf);
			if (endOfRequest(*buffer)) {
				break;
			}
		} else {
			break;
		}
	}

	return *buffer;
}

} //namespace

