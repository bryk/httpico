/**
 =========================================================================================
 File name	: /httpico/include/HttpRequest.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPICO_HTTPREQUEST_HPP_
#define HTTPICO_HTTPREQUEST_HPP_

namespace Httpico {

/*
 * HttpRequest
 */
class HttpRequest {
public:
	explicit HttpRequest(int socketFd);
	virtual ~HttpRequest();
	inline int getSocketFd() {
		return socketFd_;
	}

private:
	int socketFd_;
};
} //namespace

#endif 
