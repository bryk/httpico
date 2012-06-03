/**
 =========================================================================================
 File name	: /httpico/src/FileResponseProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "FileResponseProcessor.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Utils.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include <algorithm>

namespace Httpico {

FileResponseProcessor::FileResponseProcessor(HttpResponse &resp, HttpRequest &req, HttpServerConfiguration &conf) :
		HttpResponseProcessor(resp, req, conf) {
}

FileResponseProcessor::~FileResponseProcessor() {
}

Buffer * FileResponseProcessor::getContent() throw (std::exception) {
	Logger::getInstance().dbg("File\n");
	Buffer *ret = new Buffer();
	FILE *f = fopen(request.reqestedResourcePath.c_str(), "r");
	char buf[BUFSIZ];
	size_t rd;
	while ((rd = fread(buf, sizeof(char), BUFSIZ - 1, f))) {
		for (size_t i = 0; i < rd; i++) {
			ret->append(1, buf[i]);
		}
	}
	fclose(f);
	return ret;
}

std::string FileResponseProcessor::getContentType() {
	std::string ext = Utils::getExtenstion(request.reqestedResourcePath);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	Logger::getInstance().dbg("Rozszerzenie: %s\n", ext.c_str());
	if (ext == "" || ext == "txt") {
		return "text/plain";
	} else if (ext == "bmp") {
		return "image/bmp";
	} else if (ext == "jpg" || ext == "jpeg") {
		return "image/jpeg";
	} else if (ext == "pdf") {
		return "application/pdf";
	} else if (ext == "swf") {
		return "application/x-shockwave-flash";
	} else if (ext == "css") {
		return "text/css";
	} else if (ext == "js") {
		return "application/x-javascript";
	} else if (ext == "png") {
		return "image/png";
	} else if (ext == "gif") {
		return "image/gif";
	} else if (ext == "html" || ext == "htm") {
		return "text/html";
	}
	return "text/plain";
}

}
