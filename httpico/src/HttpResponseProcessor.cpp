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
	Buffer *buf = NULL;
	try {
		buf = getContent();
		ctype = getContentType();
	} catch (std::exception &e) {
		try {
			buf = HttpResponseProcessor::getContent();
		} catch (std::exception &e) {
			response.state = INTERNAL_SERVER_ERROR;
			buf = new Buffer();
			*buf = stateValueToString(response.state);
			*buf += " ";
			*buf += stateToString(response.state);
		}
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
	Buffer content;
	std::string p;
	p += configuration.getServerRoot();
	p += request.reqestedResourcePath;
	Utils::dbg("Requested resource:%s\n", request.reqestedResourcePath.c_str());

	std::string title = stateValueToString(response.state) + " " + stateToString(response.state);
	content += "<div class='error'>";
	content += title;
	content += "</div>";

	content += "<div class='requestInfo'><p class='title'>Request information</p><p>Method:";
	if (request.requestType == GET) {
		content += " GET";
	} else if (request.requestType == POST) {
		content += " POST";
	} else {
		content += " UNKNOWN";
	}
	content += "<br/>Requested resource: '";
	content += request.reqestedResource;
	content += "'</p>";
	if (request.getNumOfGetArgs() > 0) {
		content += "<br>GET parameters:<table><tbody>";
		for (size_t i = 0; i < request.getNumOfGetArgs(); i++) {
			content += "<tr><td class='titleColumn'>";
			content += request.getIthGetArg(i).first;
			content += "</td><td>";
			content += request.getIthGetArg(i).second;
			content += "</td>";
			content += "</tr>";
		}
		content += "</tbody></table>";
	}

	if (request.getNumOfHeaders() > 0) {
		content += "<br>Request headers:<table><tbody>";
		for (size_t i = 0; i < request.getNumOfHeaders(); i++) {
			content += "<tr><td class='titleColumn'>";
			content += request.getIthHeader(i).first;
			content += "</td><td>";
			content += request.getIthHeader(i).second;
			content += "</td>";
			content += "</tr>";
		}
		content += "</tbody></table>";
	}
	content += "</div>";
	return Utils::getTempatedHtmlFile(title, content, configuration);
}

std::string HttpResponseProcessor::getContentType() {
	return "text/html";
}

}
