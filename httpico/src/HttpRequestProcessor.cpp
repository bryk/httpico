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
	txt += request->clientAddress + " " + HttpRequest::requestTypeToString(request->requestType) + " ";
	txt += "'" + request->reqestedResource + "'";
	txt += " -> ";
	txt += stateValueToString(response->state) + " " + stateToString(response->state);
	txt += ", transferred " + Utils::toString(response->bytesTransferred) + " bytes";

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
	if (!parseRequest(buf) || httpRequest->requestType == UNKNOWN) {
		httpResponse->state = INTERNAL_SERVER_ERROR;
		Logger::getInstance().dbg("INTERNAL SERVER ERROR\n");
	} else {
		Logger::getInstance().dbg("Requested resource:%s\n", httpRequest->reqestedResource.c_str());
		if (httpRequest->reqestedResourcePath == "") {
			httpResponse->state = NOT_FOUND;
		} else {
			struct stat buf;
			if (stat(httpRequest->reqestedResourcePath.c_str(), &buf) < 0) {
				Logger::getInstance().dbg("stat() error %s: %s\n", httpRequest->reqestedResourcePath.c_str(),
						sys_errlist[errno]);
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
		std::vector<std::string> toks = Utils::tokenize(args, "&");
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
	if ((rpathBuf = canonicalize_file_name(path.c_str())) == NULL) { //let's change to real path
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
			std::vector<std::string> token = Utils::tokenize(line);
			if (token.size() < 2) {
				Logger::getInstance().dbg("Error. Size: %d, val:%s\n", token.size(), token[0].c_str());
				return false; //todo
			} else {
				if (token[0] == "GET") {
					httpRequest->requestType = GET;
				} else if (token[0] == "POST") {
					httpRequest->requestType = POST;
				} else {
					httpRequest->requestType = UNKNOWN;
					Logger::getInstance().dbg("Unknown method: '%s'\n", token[0].c_str());
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
					value = Utils::trim(line.substr(semPos + 1));
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
