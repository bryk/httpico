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

namespace Utils {

void initSignalHandlers();
bool shouldExit();
void setShouldExit();
void log(const char *fmt, ...);
void dbg(const char *fmt, ...);
std::string urlDecode(const std::string &);

template<class T>
std::string toString(const T& val) {
	std::ostringstream out;
	out << val;
	return out.str();
}

int toInt(const std::string & val, size_t base);
std::string getExtenstion(const std::string & val);

}
#endif 
