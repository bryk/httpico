/**
 =========================================================================================
 File name	: /httpico/include/HttpRequest.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPICO_HTTPREQUEST_HPP_
#define HTTPICO_HTTPREQUEST_HPP_
#include "HttpRequestState.hpp"
#include <boost/shared_ptr.hpp>
#include "HttpServer.hpp"

namespace Httpico {

/*
 * HttpRequest
 */
class HttpRequest {
public:
	explicit HttpRequest(socketPtr sock);
	virtual ~HttpRequest();
	inline HttpRequestState getState() {
		return state;
	}

	inline void setState(HttpRequestState st) {
		state = st;
	}

	inline socketPtr getSocket() {
		return sock;
	}

private:
	HttpRequestState state;
	socketPtr sock;
};
} //namespace

#endif 
