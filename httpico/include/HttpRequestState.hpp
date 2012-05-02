/**
 =========================================================================================
 File name	: /httpico/include/HttpRequestState.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPREQUESTSTATE_HPP_
#define HTTPREQUESTSTATE_HPP_

namespace Httpico {

enum HttpRequestState {
	JUST_CREATED, READING_FROM_SOCKET, READ_FROM_SOCKET, PROCESSING_RESPONSE, PROCESSED_RESPONSE
};

} //namespace

#endif 
