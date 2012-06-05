/**
 =========================================================================================
 File name	: /httpico/include/Utils.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <sstream>
#include "ForwardDeclarations.hpp"
#include <exception>
#include <vector>

namespace Utils {

void initSignalHandlers();
bool shouldExit();
void setShouldExit();
std::string urlDecode(const std::string &str);
std::string urlEncode(const std::string &str);
std::string getTimestamp();

bool registerThread();
void unregisterThread();
void waitForAllThreadsToTerminate();

template<class T>
std::string toString(const T& val) {
	std::ostringstream out;
	out << val;
	return out.str();
}

std::string trim(const std::string &str);
inline bool isIn(char c, const std::string &del);
std::vector<std::string> tokenize(const std::string &line, const std::string del = "\n\r\t ");

std::string getExtenstion(const std::string & val);
std::string *getTempatedHtmlFile(const std::string &title, const std::string &content,
		Httpico::HttpServerConfiguration &conf) throw (std::exception);
}
#endif 
