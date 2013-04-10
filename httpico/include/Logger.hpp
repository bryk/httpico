/**
 =========================================================================================
 File name	: /httpico/include/Logger.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "ForwardDeclarations.hpp"
#include <cstdio>

namespace Httpico {

/*
 * Httpico::Logger
 */
class Logger {
public:
	static Logger &getInstance();
	virtual ~Logger();
	Logger(HttpServerConfiguration &);
	void log(const char *fmt, ...);
	void dbg(const char *fmt, ...);
	static void fallbackLog(const char *fmt, ...);
private:
	FILE *logFile;
	static Logger *instance;
	HttpServerConfiguration &conf;
	Logger(const Logger &);
	Logger& operator=(const Logger&);
};

} //namespace Httpico
#endif 
