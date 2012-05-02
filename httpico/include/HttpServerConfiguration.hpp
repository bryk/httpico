/**
 =========================================================================================
 File name	: /httpico/include/HttpServerConfiguration.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPSERVERCONFIGURATION_HPP_
#define HTTPSERVERCONFIGURATION_HPP_

namespace Httpico {

/*
 * Httpico::HttpServerConfiguration
 */
class HttpServerConfiguration {
public:
	HttpServerConfiguration();
	virtual ~HttpServerConfiguration();
	int getServerPort();
	void setServerPort(int p);
	int getRequestBufferSize();
	void setRequestBufferSize(int p);
private:
	int serverPort;
	int requestBufferSize;

	static const int DEFAULT_SERVER_PORT = 8888;
	static const int REQUEST_BUFFER_SIZE = 1024 * 8;
};

}
#endif 
