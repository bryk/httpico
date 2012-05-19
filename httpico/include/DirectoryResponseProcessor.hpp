/**
 =========================================================================================
 File name	: /httpico/src/DirectoryResponseProcessor.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef DIRECTORYRESPONSEPROCESSOR_HPP_
#define DIRECTORYRESPONSEPROCESSOR_HPP_

#include "HttpResponseProcessor.hpp"

namespace Httpico {

/*
 * Httpico::DirectoryResponseProcessor
 */
class DirectoryResponseProcessor: public Httpico::HttpResponseProcessor {
public:
	DirectoryResponseProcessor(HttpResponse &resp, HttpRequest &request, HttpServerConfiguration &conf);
	virtual ~DirectoryResponseProcessor();
protected:
	virtual Buffer *getContent() throw (std::exception);
};

}
#endif 
