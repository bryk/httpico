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
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include "Logger.hpp"

namespace Httpico {

namespace {

bool isIn(char c, const std::string &del) {
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

}

HttpRequestProcessor::HttpRequestProcessor(HttpRequest * httpRequest, HttpResponse *httpResponse,
		HttpServerConfiguration &configuration) :
		httpRequest(httpRequest), httpResponse(httpResponse), configuration(configuration) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
	Logger::getInstance().dbg("Niszcze processora\n");
	delete httpRequest;
	delete httpResponse;
}

void HttpRequestProcessor::process() {
	Logger::getInstance().dbg("Processuje requesta\n");
	Buffer &buf = httpRequest->readRequest();
	HttpResponseProcessor *processor = NULL;
	if (!parseRequest(buf)) {
		httpResponse->state = INTERNAL_SERVER_ERROR;
		Logger::getInstance().dbg("INTERNAL SERVER ERROR\n");
	} else {
		Logger::getInstance().dbg("Requested resource:%s\n", httpRequest->reqestedResourcePath.c_str());
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
		Logger::getInstance().dbg("Nie plik ani folder\n");
		processor = new HttpResponseProcessor(*httpResponse, *httpRequest, configuration);
	}
	processor->process();
	delete processor;
	Logger::getInstance().dbg("Skończyłem processować requesta\n");
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

	Logger::getInstance().dbg("Requested resource:%s\n", httpRequest->reqestedResource.c_str());
	std::string path;
	path += configuration.getServerRoot();
	path += httpRequest->reqestedResource;
	char *rpathBuf;
	if ((rpathBuf = realpath(path.c_str(), NULL)) == NULL) { //let's change to real path
		perror(path.c_str());
		httpRequest->reqestedResourcePath = "";
	} else {
		httpRequest->reqestedResourcePath = rpathBuf;
	}
	Logger::getInstance().dbg("Requested resource PATH:%s\n", httpRequest->reqestedResourcePath.c_str());
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
		//Utils::dbg("Wczytano linię: %s", line.c_str());
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
					Logger::getInstance().dbg("Zła metoda: %s", token[0].c_str());
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
