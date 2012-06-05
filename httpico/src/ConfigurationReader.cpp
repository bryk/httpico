/**
 =========================================================================================
 File name	: /httpico/include/ConfigurationReader.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "ConfigurationReader.hpp"
#include <cstdio>
#include "Logger.hpp"
#include "Utils.hpp"
#include <cstdlib>
#include <vector>
#include "HttpServerConfiguration.hpp"

namespace Httpico {

namespace {
bool fillOption(std::string &key, std::string &val, HttpServerConfiguration & conf) {
	if (key == "server_root") {
		conf.setServerRoot(val);
	} else if (key == "server_port") {
		conf.setServerPort(atoi(val.c_str()));
	} else if (key == "request_buffer_size") {
		conf.setRequestBufferSize(atoi(val.c_str()));
	} else if (key == "server_template_root") {
		conf.setServerTemplateRoot(val);
	} else if (key == "logging_directory") {
		conf.setLoggingFolder(val);
	} else {
		return false;
	}
	return true;
}
}

ConfigurationReader::ConfigurationReader(FILE * f) :
		file(f) {
}

bool ConfigurationReader::fillConfig(HttpServerConfiguration &conf) {
	fseek(file, 0, SEEK_SET);
	size_t nbytes = BUFSIZ;
	char *buf = new char[nbytes];
	size_t bytes_read;
	while ((bytes_read = getline(&buf, &nbytes, file)) != -1) {
		std::string line = buf;
		line = Utils::trim(line);
		if (line.size() == 0 || line[0] == '#') {
			continue;
		}
		std::vector<std::string> token = Utils::tokenize(line, "=");
		if (token.size() != 2) {
			return false;
		} else {
			token[0] = Utils::trim(token[0]);
			token[1] = Utils::trim(token[1]);
			if (fillOption(token[0], token[1], conf) == false) {
//				Logger::fallbackLog("Wrong configuration option: %s\n", token[0].c_str());
				return false;
			}
		}
	}
	delete[] buf;
	return true;
}

ConfigurationReader::~ConfigurationReader() {
}

} //namespace Httpico
