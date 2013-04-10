/**
 =========================================================================================
 File name	: /httpico/src/FileResponseProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef FILERESPONSEPROCESSOR_HPP_
#define FILERESPONSEPROCESSOR_HPP_

#include "HttpResponseProcessor.hpp"

namespace Httpico {

/*
 * Httpico::FileResponseProcessor
 */
class FileResponseProcessor: public Httpico::HttpResponseProcessor {
public:
	FileResponseProcessor(HttpResponse &resp, HttpRequest &request, HttpServerConfiguration &conf);
	virtual ~FileResponseProcessor();
protected:
	virtual Buffer * getContent() throw (std::exception);
	virtual std::string getContentType();
};

}
#endif 
