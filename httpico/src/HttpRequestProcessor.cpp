/**
 =========================================================================================
 File name	: /httpico/src/HttpRequestProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpRequestProcessor.hpp"
#include "Utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <string.h>
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

HttpRequestProcessor::HttpRequestProcessor(HttpRequest * httpRequest, HttpResponse *httpResponse) :
		httpRequest_(httpRequest), httpResponse_(httpResponse) {
}

HttpRequestProcessor::~HttpRequestProcessor() {
	Utils::dbg("Niszcze processora\n");
	delete httpRequest_;
	delete httpResponse_;
}

void HttpRequestProcessor::process() {
	Utils::dbg("Processuje requesta\n");
	Buffer &buf = httpRequest_->readRequest();
	parseRequest(buf);
	std::string resp;
	resp += "HTTP/1.0 200 OK\r\nServer: httpico\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";
	resp += "Cześć, zapytałeś się mnie metodą";
	if (httpRequest_->requestType == GET) {
		resp += " GET";
	} else if (httpRequest_->requestType == POST) {
		resp += " POST";
	}
	resp += "<br>Chciałeś mieć dostęp do zasobu: '";
	resp += httpRequest_->reqestedResource;
	resp += "'<br><br>Wywołałeś stronę z argumentami GET:<br>";
	for (size_t i = 0; i < httpRequest_->getNumOfGetArgs(); i++) {
		resp += httpRequest_->getIthGetArg(i).first;
		resp += " = ";
		resp += httpRequest_->getIthGetArg(i).second;
		resp += "<br>";
	}

	resp += "<br><br>Wywołałeś stronę z nagłówakmi:<br>";
	for (size_t i = 0; i < httpRequest_->getNumOfHeaders(); i++) {
		resp += httpRequest_->getIthHeader(i).first;
		resp += " = ";
		resp += httpRequest_->getIthHeader(i).second;
		resp += "<br>";
	}
	httpResponse_->writeResponse(resp);
}

void HttpRequestProcessor::parseResourceName(const std::string &res) {
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
}

void HttpRequestProcessor::parseRequest(const std::string &buf) {
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
				//TODO
			} else {
				if (token[0] == "GET") {
					httpRequest_->requestType = GET;
				} else if (token[0] == "POST") {
					httpRequest_->requestType = POST;
				} else {
					//TODO;
					Utils::dbg("Zła metoda: %s", token[0].c_str());
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
}

}
