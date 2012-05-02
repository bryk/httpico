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

namespace Httpico {

/*
 * HttpRequest
 */
class HttpRequest {
public:
	HttpRequest();
	virtual ~HttpRequest();
	inline HttpRequestState getState() {
		return state;
	}

	inline void setState(HttpRequestState st) {
		state = st;
	}
private:
	HttpRequestState state;
};

typedef boost::shared_ptr<HttpRequest> HttpRequestPtr;

} //namespace

#endif 
