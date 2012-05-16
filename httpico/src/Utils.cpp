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

void initShouldExit() {
	shouldExit_ = 0;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = sigIntHandler;
	if (sigaction(SIGINT, &sact, NULL) == -1) {
		perror(NULL);
	}
}

bool shouldExit() {
	return shouldExit_;
}

}
