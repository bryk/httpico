/**
 =========================================================================================
 File name	: /httpico/src/Utils.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "Utils.hpp"
#include <csignal>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>

namespace Utils {

namespace {
volatile sig_atomic_t shouldExit_;

void sigIntHandler(int signo) {
	shouldExit_ = 1;
}

void sigAlrmHandler(int signo) {
}

void logToFile(FILE *f, const char *fmt, va_list args) {
	char dateBuf[26];
	time_t currTime;
	time(&currTime);
	strftime(dateBuf, 26, "%H:%M:%S", localtime(&(currTime)));

	std::ostringstream out;
	out << "(" << dateBuf << "): " << fmt;

	vfprintf(f, out.str().c_str(), args);
}

} //namespace

std::string getExtenstion(const std::string & val) {
	size_t pos = val.rfind('.');
	if (pos != std::string::npos) {
		return val.substr(pos + 1);
	}
	return "";
}

int toInt(const std::string & val, size_t base) {
	int ret = 0, pow = 1;
	for (int i = val.size() - 1; i >= 0; i--) {
		ret += pow * (val[i] - '0');
		pow *= base;
	}
	return ret;
}

std::string urlDecode(const std::string &str) {
	std::string ret, byte;
	size_t i = 0;
	while (i < str.size()) {
		if (str[i] == '%' && i + 2 < str.size()) {
			byte = "";
			byte += str[i + 1];
			byte += str[i + 2];
			Utils::dbg("Bajt: %s zmieniłem na '%c' (%d)\n", byte.c_str(), (char) toInt(byte, 16), (char) toInt(byte, 16));
			ret += (char) toInt(byte, 16);
			i += 3;
		} else {
			ret += str[i];
			i++;
		}
	}
	return ret;
}

void dbg(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	logToFile(stderr, fmt, args);
	va_end(args);
}

void log(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	logToFile(stdout, fmt, args);
	va_end(args);
}

void setShouldExit() {
	shouldExit_ = 1;
}

void sigpipeHandler(int) {
	Utils::dbg("SIGPIPE\n");
	//nothing
}

void initSignalHandlers() {
	shouldExit_ = 0;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = sigIntHandler;
	if (sigaction(SIGINT, &sact, NULL) == -1) {
		perror(NULL);
	}
	signal(SIGPIPE, sigpipeHandler);
}

bool shouldExit() {
	return shouldExit_;
}

}
