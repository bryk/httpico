/**
 =========================================================================================
 File name	: /httpico/include/HttpResponse.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "HttpResponseState.hpp"
#include <string>

namespace Httpico {

typedef std::string Buffer;
/*
 * Httpico::HttpResponse
 */
class HttpResponse {
public:
	explicit HttpResponse(int socketFd);
	virtual ~HttpResponse();
	void writeResponse(const Buffer &buf);
	HttpResponseState state;
	int bytesTransferred;
private:
	int socketFd_;
};

}
#endif 
