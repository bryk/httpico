/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor.hpp"
#include "Utils.hpp"
#include "HttpRequest.hpp"
#include "HttpServerConfiguration.hpp"
#include "HttpResponse.hpp"
#include "HttpResponseProcessor.hpp"
#include "DirectoryResponseProcessor.hpp"
#include "FileResponseProcessor.hpp"
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "pthread.h"
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include "Logger.hpp"

namespace Httpico {

namespace { //unnamed namespace
inline bool isIn(char c, const std::string &del) {
	if (del.find(c) != std::string::npos) {
		return true;
	} else {
		return false;
	}
}

std::string trim(const std::string &str) {
	size_t a = 0, b = str.size();
	std::string del("\n\r\t ");
	while (a < str.size() && isIn(str[a], del)) {
		a++;
	}
	while (b > a && isIn(str[b], del)) {
		b--;
	}
	return str.substr(a, b - a);
}

std::vector<std::string> tokenize(const std::string &line, const std::string del = "\n\r\t ") {
	size_t a = 0, b = 0;
	std::vector<std::string> ret;
	while (a < line.size() && isIn(line[a], del)) {
		a++;
	}
	while (a < line.size()) {
		b = a;
		while (b < line.size() && !isIn(line[b], del)) {
			b++;
		}
		ret.push_back(line.substr(a, b - a));
		a = b;
		while (a < line.size() && isIn(line[a], del)) {
			a++;
		}
	}
	return ret;
}

void *run(void * arg) {
	pthread_detach(pthread_self());
	Utils::initSignalHandlers();
	if (!Utils::registerThread()) {
		return NULL;
	}
	HttpRequestProcessor *processor = (HttpRequestProcessor*) arg;
	processor->process();
	delete processor;
	Utils::unregisterThread();
	return NULL;
}

void logRequest(HttpRequest *request, HttpResponse *response) {
	std::string txt;
	txt += request->reqestedResource + " -> ";
	txt += stateValueToString(response->state) + " " + stateToString(response->state);
	txt += ", transfered " + Utils::toString(response->bytesTransferred) + " bytes";

	Logger::getInstance().log("%s\n", txt.c_str());
}

} //namespace

HttpRequestProcessor::HttpRequestProcessor(HttpRequest * httpRequest, HttpResponse *httpResponse,
		HttpServerConfiguration &configuration) :
		httpRequest(httpRequest), httpResponse(httpResponse), configuration(configuration) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
	delete httpRequest;
	delete httpResponse;
}

void HttpRequestProcessor::process() {
	Buffer &buf = httpRequest->readRequest();
	HttpResponseProcessor *processor = NULL;
	if (!parseRequest(buf)) {
		httpResponse->state = INTERNAL_SERVER_ERROR;
		Logger::getInstance().dbg("INTERNAL SERVER ERROR\n");
	} else {
		Logger::getInstance().dbg("Requested resource:%s\n", httpRequest->reqestedResource.c_str());
		if (httpRequest->reqestedResourcePath == "") { //let's change to real path
			httpResponse->state = NOT_FOUND;
		} else {
			struct stat buf;
			if (stat(httpRequest->reqestedResourcePath.c_str(), &buf) < 0) {
				perror(httpRequest->reqestedResourcePath.c_str());
			}
			if (S_ISDIR(buf.st_mode)) {
				processor = new DirectoryResponseProcessor(*httpResponse, *httpRequest, configuration);
			} else {
				processor = new FileResponseProcessor(*httpResponse, *httpRequest, configuration);
			}
			httpResponse->state = OK;
		}
	}
	if (processor == NULL) {
		processor = new HttpResponseProcessor(*httpResponse, *httpRequest, configuration);
	}
	processor->process();
	logRequest(httpRequest, httpResponse);
	delete processor;
}

void HttpRequestProcessor::handleRequest() {
	pthread_t tid;
	if (pthread_create(&tid, NULL, run, this) != 0) {
		Logger::getInstance().dbg("Couldn't run new thread\n");
		delete this; //suicide
	}
}

bool HttpRequestProcessor::parseResourceName(const std::string &res) {
	size_t qmark = res.find('?');
	if (qmark != std::string::npos) {
		httpRequest->reqestedResource = res.substr(0, qmark);
		std::string args = res.substr(qmark + 1);
		std::vector<std::string> toks = tokenize(args, "&");
		for (size_t i = 0; i < toks.size(); i++) {
			size_t eqPos = toks[i].find('=');
			std::string key;
			std::string value;
			if (eqPos != std::string::npos) {
				key = toks[i].substr(0, eqPos);
				value = toks[i].substr(eqPos + 1);
			} else {
				key = toks[i];
			}
			httpRequest->setGetArg(key, value);
		}
	} else {
		httpRequest->reqestedResource = res;
	}
	httpRequest->reqestedResource = Utils::urlDecode(httpRequest->reqestedResource);

	std::string path;
	path += configuration.getServerRoot();
	path += httpRequest->reqestedResource;
	char *rpathBuf;
	if ((rpathBuf = realpath(path.c_str(), NULL)) == NULL) { //let's change to real path
		Logger::getInstance().dbg("%s: %s\n", path.c_str(), sys_errlist[errno]);
		httpRequest->reqestedResourcePath = "";
	} else {
		httpRequest->reqestedResourcePath = rpathBuf;
	}
	free(rpathBuf);
	return true;
}

bool HttpRequestProcessor::parseRequest(const std::string &buf) {
	size_t a = 0, lineNum = 0;
	while (a < buf.size()) {
		size_t b = a;
		while (b < buf.size() && buf[b] != '\n') {
			b++;
		}
		std::string line = buf.substr(a, b - a + 1);
		if (lineNum == 0) {
			std::vector<std::string> token = tokenize(line);
			if (token.size() < 2) {
				return false; //todo
			} else {
				if (token[0] == "GET") {
					httpRequest->requestType = GET;
				} else if (token[0] == "POST") {
					httpRequest->requestType = POST;
				} else {
					return false; //todo
				}
				parseResourceName(token[1]);
			}
		} else {
			if (line.size() != 2) {
				size_t semPos = line.find(':');
				std::string key;
				std::string value;
				if (semPos != std::string::npos) {
					key = line.substr(0, semPos);
					value = trim(line.substr(semPos + 1));
				} else {
					key = line;
				}
				httpRequest->setHeader(key, value);
			}
		}
		a = b + 1;
		lineNum++;
	}
	return true;
}

}
