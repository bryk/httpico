/**
 =========================================================================================
 File name	: /httpico/include/HttpServerConfiguration.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPSERVERCONFIGURATION_HPP_
#define HTTPSERVERCONFIGURATION_HPP_

#include <string>

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
	const std::string &getServerRoot();
	void setServerRoot(const std::string &);
	const std::string &getServerTemplateRoot();
	void setServerTemplateRoot(const std::string &);
private:
	int serverPort;
	int requestBufferSize;
	std::string serverRoot;
	std::string serverTemplateRoot;
	static const int DEFAULT_SERVER_PORT = 8888;
	static const int REQUEST_BUFFER_SIZE = 1024 * 8;
};

}
#endif 
