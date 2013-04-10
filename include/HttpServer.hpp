/**
 =========================================================================================
 File name	: /httpico/include/HttpServer.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPSERVER_HPP_
#define HTTPSERVER_HPP_

#include "ForwardDeclarations.hpp"

namespace Httpico {

/*
 * Httpico::HttpServer
 */
class HttpServer {
public:
	HttpServer(HttpServerConfiguration &conf);
	virtual ~HttpServer();
	void start();

private:
	HttpServerConfiguration &configuration;
	void initialize();
	int socketFd;
};

}
#endif 
