/**
 =========================================================================================
 File name	: /httpico/src/HttpServerConfiguration.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "HttpServerConfiguration.hpp"
#include "config.hpp"
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include "Utils.hpp"
#include "Logger.hpp"

namespace Httpico {

#ifndef EMBEDDED
#define DEFAULT_SERVER_ROOT "/usr/share/httpico/srv"
#define DEFAULT_SERVER_TEMPLATE_ROOT "/usr/share/httpico/"
#define DEFAULT_LOGGING_DIRECTORY "/usr/share/httpico/log"
#define DEFAULT_CONFIGURATION_DIRECTORY "/etc/"
#else
#define DEFAULT_SERVER_ROOT "./"
#define DEFAULT_SERVER_TEMPLATE_ROOT "./"
#define DEFAULT_LOGGING_DIRECTORY "./"
#define DEFAULT_CONFIGURATION_DIRECTORY "./"
#endif

HttpServerConfiguration::HttpServerConfiguration() :
		serverPort(DEFAULT_SERVER_PORT), requestBufferSize(REQUEST_BUFFER_SIZE) {
	setConfigurationDirectory(DEFAULT_CONFIGURATION_DIRECTORY);
	setServerRoot(DEFAULT_SERVER_ROOT);
	setServerTemplateRoot(DEFAULT_SERVER_TEMPLATE_ROOT);
	setLoggingFolder(DEFAULT_LOGGING_DIRECTORY);
}

HttpServerConfiguration::~HttpServerConfiguration() {
}

void HttpServerConfiguration::setConfigurationDirectory(const std::string &srv) {
	char *rpathBuf;
	if ((rpathBuf = canonicalize_file_name(srv.c_str())) == NULL) { //let's change to real path
		Logger::fallbackLog("Error while setting configuration root %s: %s", srv.c_str(), sys_errlist[errno]);
		exit(EXIT_FAILURE);
	} else {
		configurationDirectory = rpathBuf;
	}
	free(rpathBuf);
}

const std::string &HttpServerConfiguration::getServerTemplateRoot() {
	return serverTemplateRoot;
}

const std::string &HttpServerConfiguration::getConfigurationDirectory() {
	return configurationDirectory;
}

void HttpServerConfiguration::setServerTemplateRoot(const std::string &srv) {
	char *rpathBuf;
	if ((rpathBuf = canonicalize_file_name(srv.c_str())) == NULL) { //let's change to real path
		Logger::fallbackLog("Error while setting template root %s: %s", srv.c_str(), sys_errlist[errno]);
		exit(EXIT_FAILURE);
	} else {
		serverTemplateRoot = rpathBuf;
	}
	free(rpathBuf);
}

const std::string &HttpServerConfiguration::getLoggingFolder() {
	return loggingFolder;
}

void HttpServerConfiguration::setLoggingFolder(const std::string & log) {
	char *rpathBuf;
	if ((rpathBuf = canonicalize_file_name(log.c_str())) == NULL) { //let's change to real path
		Logger::fallbackLog("Error while setting logging directory %s: %s", log.c_str(), sys_errlist[errno]);
		exit(EXIT_FAILURE);
	} else {
		loggingFolder = rpathBuf;
	}
	free(rpathBuf);
}

const std::string &HttpServerConfiguration::getServerRoot() {
	return serverRoot;
}

void HttpServerConfiguration::setServerRoot(const std::string &srv) {
	char *rpathBuf;
	if ((rpathBuf = canonicalize_file_name(srv.c_str())) == NULL) { //let's change to real path
		Logger::fallbackLog("Error while setting server root %s: %s", srv.c_str(), sys_errlist[errno]);
		exit(EXIT_FAILURE);
	} else {
		serverRoot = rpathBuf;
	}
	free(rpathBuf);
}

int HttpServerConfiguration::getServerPort() {
	return serverPort;
}

void HttpServerConfiguration::setServerPort(int p) {
	serverPort = p;
}

int HttpServerConfiguration::getRequestBufferSize() {
	return requestBufferSize;
}

void HttpServerConfiguration::setRequestBufferSize(int p) {
	requestBufferSize = p;
}

}
