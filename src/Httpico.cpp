/**
 =========================================================================================
 File name	: httpico.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include "HttpRequest.hpp"
#include "HttpServer.hpp"
#include "HttpServerConfiguration.hpp"
#include "ConfigurationReader.hpp"
#include "Logger.hpp"
#include <cerrno>

using namespace std;
using namespace Httpico;

int main(void) {
	HttpServerConfiguration conf;
	std::string name = conf.getConfigurationDirectory() + "/httpico.conf";
	FILE *confFile = fopen(name.c_str(), "r");
	if (!confFile) {
		Logger::fallbackLog("Couldn't read configuration file %s: %s\n", name.c_str(), sys_errlist[errno]);
		return EXIT_FAILURE;
	} else {
		ConfigurationReader creader(confFile);
		if (creader.fillConfig(conf)) {
			HttpServer server(conf);
			server.start();
			return EXIT_SUCCESS;
		} else {
			Logger::fallbackLog("Wrong configuration file\n");
		}
	}
}
