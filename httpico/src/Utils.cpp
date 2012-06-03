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
#include "HttpServerConfiguration.hpp"
#include <string>
#include "Logger.hpp"
#include <sys/time.h>
#include "pthread.h"

namespace Utils {

namespace {
volatile sig_atomic_t shouldExit_;

void sigIntHandler(int signo) {
	shouldExit_ = 1;
}

void sigAlrmHandler(int signo) {
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int noThreads = 0;
volatile bool dontAllow = false;

} //namespace

bool registerThread() {
	pthread_mutex_lock(&mutex);
	if (dontAllow) {
		pthread_mutex_unlock(&mutex);
		return false;
	} else {
		noThreads++;
		pthread_mutex_unlock(&mutex);
		return true;
	}
}

void unregisterThread() {
	pthread_mutex_lock(&mutex);
	noThreads--;
	pthread_mutex_unlock(&mutex);
}

void waitForAllThreadsToTerminate() {
	int i = 2000; // two seconds
	Httpico::Logger::getInstance().dbg("Waiting for other threads to terminate\n");
	while (1) {
		pthread_mutex_lock(&mutex);
		if (noThreads == 0) {
			dontAllow = true;
			pthread_mutex_unlock(&mutex);
			return;
		} else {
			pthread_mutex_unlock(&mutex);
			usleep(1000);
			if (i-- == 0) {
				return;
			}
		}
	}
}

std::string getTimestamp() {
	char dateBuf[26];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(dateBuf, 26, "%H:%M:%S", localtime(&(tv.tv_sec)));
	std::string ret(dateBuf);
	ret += "." + toString((int) (tv.tv_usec / 1000));
	return ret;
}

std::string *getTempatedHtmlFile(const std::string &title, const std::string &content,
		Httpico::HttpServerConfiguration &conf) throw (std::exception) {
	std::string cont;
	FILE *f = fopen((conf.getServerTemplateRoot() + "/template.html").c_str(), "r");
	if (!f) {
		throw std::exception();
	}
	char buf[BUFSIZ];
	size_t rd;
	while ((rd = fread(buf, sizeof(char), BUFSIZ - 1, f))) {
		for (size_t i = 0; i < rd; i++) {
			cont.append(1, buf[i]);
		}
	}
	fclose(f);
	std::string titleMark = "{title}";
	std::string contentMark = "{content}";
	std::string *ret = new std::string();
	size_t pos = cont.find(titleMark);
	if (pos == std::string::npos) {
		delete ret;
		throw std::exception();
	}
	*ret = cont.substr(0, pos);
	*ret += title;
	size_t startPos = cont.find(titleMark);
	size_t endPos = cont.find(contentMark);
	if (startPos == std::string::npos || endPos == std::string::npos) {
		delete ret;
		throw std::exception();
	}
	startPos += titleMark.size();
	for (size_t i = startPos; i < endPos; i++) {
		*ret += cont[i];
	}
	*ret += content;

	startPos = cont.find(contentMark) + contentMark.size();
	endPos = cont.size();
	if (startPos == std::string::npos || endPos == std::string::npos) {
		delete ret;
		throw std::exception();
	}
	for (size_t i = startPos; i < endPos; i++) {
		*ret += cont[i];
	}
	return ret;
}

std::string getExtenstion(const std::string & val) {
	size_t pos = val.rfind('.');
	if (pos != std::string::npos) {
		return val.substr(pos + 1);
	}
	return "";
}

int hexInt(const std::string & val, size_t base) {
	int ret = 0, pow = 1;
	std::string digits = "0123456789abcdef";
	for (int i = val.size() - 1; i >= 0; i--) {
		int num = (
				val[i] >= '0' && val[i] <= '9' ?
						val[i] - '0' : (val[i] >= 'a' && val[i] <= 'f' ? val[i] - 'a' + 10 : val[i] - 'A' + 10));
		ret += pow * (num);
		pow *= base;
	}
	return ret;
}

std::string urlEncode(const std::string &str) {
	std::string ret;
	std::string digits = "0123456789abcdef";
	std::string unsafe = "%=\"<>\\^[]`+$,@:;!#?&'";
	bool wasUnsafe;
	for (size_t i = 0; i < str.size(); i++) {
		wasUnsafe = false;
		for (size_t j = 0; j < unsafe.size(); j++) {
			if (str[i] == unsafe[j]) {
				ret += "%";
				char num = str[i];
				int r = num % 16;
				int l = (num / 16) % 16;
				ret += digits[l];
				ret += digits[r];
				wasUnsafe = true;
				break;
			}
		}
		if (!wasUnsafe) {
			ret += str[i];
		}
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
			ret += (char) hexInt(byte, 16);
			i += 3;
		} else {
			ret += str[i];
			i++;
		}
	}
	return ret;
}

void setShouldExit() {
	shouldExit_ = 1;
}

void sigpipeHandler(int) {
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
