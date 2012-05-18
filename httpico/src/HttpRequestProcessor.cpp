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
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

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
		httpRequest_(httpRequest), httpResponse_(httpResponse), configuration_(configuration) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
	Utils::dbg("Niszcze processora\n");
	delete httpRequest_;
	delete httpResponse_;
}

void HttpRequestProcessor::process() {
	Utils::dbg("Processuje requesta\n");
	Buffer &buf = httpRequest_->readRequest();
	if (!parseRequest(buf)) {
		httpResponse_->state = INTERNAL_SERVER_ERROR;
		Utils::dbg("INTERNAL SERVER ERROR\n");
	} else {
		std::string p;
		p += configuration_.getServerRoot();
		p += httpRequest_->reqestedResource;
		Utils::dbg("Requested resource:%s\n", httpRequest_->reqestedResource.c_str());
		char tmp[10000];
		if (realpath(p.c_str(), tmp) == NULL) { //let's change to real path
			perror(p.c_str());
			httpResponse_->state = NOT_FOUND;
		} else {
			Utils::dbg("Good path:'%s' (było: %s)\n", tmp, p.c_str());
			httpResponse_->state = OK;
		}
	}

	HttpResponseProcessor *processor = new HttpResponseProcessor(*httpResponse_, *httpRequest_, configuration_);
	processor->process();
	delete processor;
	Utils::dbg("Skończyłem processować requesta\n");
}

bool HttpRequestProcessor::parseResourceName(const std::string &res) {
	size_t qmark = res.find('?');
	if (qmark != std::string::npos) {
		httpRequest_->reqestedResource = res.substr(0, qmark);
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
			httpRequest_->setGetArg(key, value);
		}
	} else {
		httpRequest_->reqestedResource = res;
	}
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
					httpRequest_->requestType = GET;
				} else if (token[0] == "POST") {
					httpRequest_->requestType = POST;
				} else {
					Utils::dbg("Zła metoda: %s", token[0].c_str());
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
				httpRequest_->setHeader(key, value);
			}
		}
		a = b + 1;
		lineNum++;
	}
	return true;
}

}
