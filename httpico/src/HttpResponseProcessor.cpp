/**
 =========================================================================================
 File name	: /httpico/include/HttpResponseProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpResponseProcessor.hpp"
#include "HttpResponseState.hpp"
#include "HttpResponse.hpp"
#include "HttpServerConfiguration.hpp"
#include "Utils.hpp"
#include <string>
#include "HttpRequest.hpp"
#include <cstdio>
#include <cstdlib>

namespace Httpico {

HttpResponseProcessor::HttpResponseProcessor(HttpResponse &resp, HttpRequest &req, HttpServerConfiguration &conf) :
		response(resp), request(req), configuration(conf) {
}

HttpResponseProcessor::~HttpResponseProcessor() {
	// TODO Auto-generated destructor stub
}

void HttpResponseProcessor::process() {
	Utils::dbg("ResponseProcessor\n");
	std::string resp;
	std::string ctype;
	const Buffer *buf = NULL;
	try {
		buf = getContent();
		ctype = getContentType();
	} catch (std::exception &e) {
		buf = HttpResponseProcessor::getContent();
		ctype = HttpResponseProcessor::getContentType();
	}
	resp += "HTTP/1.0 " + stateValueToString(response.state);
	resp += " " + stateToString(response.state);
	resp += "\r\nServer: httpico";
	resp += "\r\nContent-Type: ";
	resp += ctype;
	resp += "\r\nConnection: close";
	resp += "\r\nContent-Length: ";
	resp += Utils::toString(buf->size());
	resp += "\r\n";
	resp += "\r\n";

	response.writeResponse(resp);
	response.writeResponse(*buf);
	delete buf;
}

Buffer *HttpResponseProcessor::getContent() throw (std::exception) {
	Utils::dbg("Zwykłe write contetn\n");
	Buffer * resp = new Buffer();
	*resp += "Cześć, zapytałeś się mnie metodą";
	if (request.requestType == GET) {
		*resp += " GET";
	} else if (request.requestType == POST) {
		*resp += " POST";
	}
	*resp += "<br>Chciałeś mieć dostęp do zasobu: '";
	//resp += httpRequest_->reqestedResource;

	std::string p;
	p += configuration.getServerRoot();
	p += request.reqestedResource;
	Utils::dbg("Requested resource:%s\n", request.reqestedResource.c_str());
	char tmp[10000];
	if (realpath(p.c_str(), tmp) == NULL) { //let's change to real path
		Utils::dbg("Zła ścieżka a:'%s', b:'%s', p:'%s'\n", configuration.getServerRoot().c_str(),
				request.reqestedResource.c_str(), p.c_str());
		perror(NULL);
	} else {
		Utils::dbg("Good path:'%s' (było: %s)\n", tmp, p.c_str());
		*resp += tmp;
	}

	*resp += "'<br><br>Wywołałeś stronę z argumentami GET:<br>";
	for (size_t i = 0; i < request.getNumOfGetArgs(); i++) {
		*resp += request.getIthGetArg(i).first;
		*resp += " = ";
		*resp += request.getIthGetArg(i).second;
		*resp += "<br>";
	}

	*resp += "<br><br>Wywołałeś stronę z nagłówakmi:<br>";
	for (size_t i = 0; i < request.getNumOfHeaders(); i++) {
		*resp += request.getIthHeader(i).first;
		*resp += " = ";
		*resp += request.getIthHeader(i).second;
		*resp += "<br>";
	}
	return resp;
}

std::string HttpResponseProcessor::getContentType() {
	return "text/html";
}

}
