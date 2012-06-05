/**
 =========================================================================================
 File name	: /httpico/include/Logger.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "Logger.hpp"
#include "HttpServerConfiguration.hpp"
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <sstream>
#include "config.hpp"
#include "Utils.hpp"

namespace Httpico {

namespace { //unnamed namespace
void logToFile(FILE *f, const char *fmt, va_list args, const char *prefix) {
	std::ostringstream out;
	out << prefix << " (" << Utils::getTimestamp() << "): " << fmt;
	vfprintf(f, out.str().c_str(), args);
	fflush(f);
}

} //namespace

Logger *Logger::instance = NULL;

void Logger::fallbackLog(const char *fmt, ...) {
	if (instance == NULL) {
		va_list args;
		va_start(args, fmt);
		logToFile(stderr, fmt, args, "FATAL ERROR");
		va_end(args);
	} else {
		va_list args;
		va_start(args, fmt);
		logToFile(stderr, fmt, args, "FATAL ERROR");
		va_end(args);
		if (instance->logFile != NULL) {
			va_start(args, fmt);
			logToFile(instance->logFile, fmt, args, "FATAL ERROR");
			va_end(args);
		}
	}
}

void Logger::log(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	logToFile(stdout, fmt, args, "INFO");
	if (logFile != NULL) {
		va_start(args, fmt);
		logToFile(logFile, fmt, args, "INFO");
		va_end(args);
	}
}

void Logger::dbg(const char *fmt, ...) {
#ifdef DEBUGLOGS
	va_list args;
	va_start(args, fmt);
	logToFile(stderr, fmt, args, "DEBUG");
	va_end(args);
	if (logFile != NULL) {
		va_start(args, fmt);
		logToFile(logFile, fmt, args, "DEBUG");
		va_end(args);
	}
#endif
}

Logger &Logger::getInstance() {
	return *instance;
}

Logger::Logger(HttpServerConfiguration &c) :
		logFile(NULL), conf(c) {
	if (instance != NULL) {
		delete instance;
	}
	std::string logFileName = conf.getLoggingFolder() + "/httpico_" + Utils::getTimestamp() + ".log";

	logFile = fopen(logFileName.c_str(), "w");
	if (logFile == NULL) {
		Logger::fallbackLog("Couldn't open logging file\n");
	}

	instance = this;
}

Logger::~Logger() {
	if (logFile != NULL) {
		fclose(logFile);
	}
}

} //namespace Httpico
