/**
 =========================================================================================
 File name	: /httpico/include/HttpRequest.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef HTTPICO_HTTPREQUEST_HPP_
#define HTTPICO_HTTPREQUEST_HPP_

#include <string>
#include <map>

namespace Httpico {

enum RequestType {
	GET, POST, UNKNOWN
};

typedef std::string Buffer;

/*
 * HttpRequest
 */
class HttpRequest {
public:
	explicit HttpRequest(int socketFd);
	virtual ~HttpRequest();

	static std::string requestTypeToString(RequestType req);

	Buffer & readRequest();
	void writeBack(Buffer &buf);
	RequestType requestType;
	std::string reqestedResourcePath;
	std::string reqestedResource;
	std::string clientAddress;

	void setGetArg(const std::string &key, const std::string &value);
	const std::string &getGetArg(const std::string &key);
	const std::pair<std::string, std::string> getIthGetArg(size_t i);
	const size_t getNumOfGetArgs();

	void setPostArg(const std::string &key, const std::string &value);
	const std::string &getPostArg(const std::string &key);
	const std::pair<std::string, std::string> getIthPostArg(size_t i);
	const size_t getNumOfPostArgs();

	void setHeader(const std::string &key, const std::string &value);
	const std::string &getHeader(const std::string &key);
	const std::pair<std::string, std::string> getIthHeader(size_t i);
	const size_t getNumOfHeaders();
private:
	int socketFd_;
	std::map<std::string, std::string> header;
	std::map<std::string, std::string> getArgs;
	std::map<std::string, std::string> postArgs;
};
} //namespace

#endif 
