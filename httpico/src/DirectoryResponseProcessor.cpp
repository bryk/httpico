/**
 =========================================================================================
 File name	: /httpico/src/DirectoryResponseProcessor.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include "DirectoryResponseProcessor.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include "HttpRequest.hpp"
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <string>
#include "HttpResponse.hpp"
#include "Utils.hpp"
#include "HttpServerConfiguration.hpp"
#include <vector>

namespace Httpico {

DirectoryResponseProcessor::DirectoryResponseProcessor(HttpResponse &resp, HttpRequest &req, HttpServerConfiguration &conf) :
		HttpResponseProcessor(resp, req, conf) {
}

DirectoryResponseProcessor::~DirectoryResponseProcessor() {
	// TODO Auto-generated destructor stub
}

Buffer * DirectoryResponseProcessor::getContent() throw (std::exception) {
	Utils::dbg("DIRECTORY\n");
	Buffer *res = new Buffer();
	struct stat buf;
	DIR *dp = opendir(request.reqestedResource.c_str());
	if (!dp) {
		perror(request.reqestedResource.c_str());
		throw std::exception();
	}
	std::vector<std::string> file;
	std::vector<std::string> fileName;
	std::vector<std::string> dir;
	std::vector<std::string> dirName;
	dir.push_back(request.reqestedResource.substr(configuration.getServerRoot().size()) + "/..");
	dirName.push_back("..");

	struct dirent * ent;
	errno = 0;
	while ((ent = readdir(dp)) != NULL) {
		if (errno) {
			perror((request.reqestedResource + " readdir").c_str());
			errno = 0;
			continue;
		}
		std::string path = ent->d_name;
		if (path == "." || path == "..") {
			continue;
		} else {
			path = request.reqestedResource + "/" + path;
			if (stat(path.c_str(), &buf) < 0) {
				perror("Blad stat");
				errno = 0;
				continue;
			}
			path = path.substr(configuration.getServerRoot().size());
			if (S_ISDIR(buf.st_mode)) {
				dir.push_back(path);
				dirName.push_back(ent->d_name);
			} else if (S_ISREG(buf.st_mode)) {
				file.push_back(path);
				fileName.push_back(ent->d_name);
			}
		}
	}
	if (closedir(dp) < 0) {
		perror(request.reqestedResource.c_str());
		errno = 0;
	}

	*res += "<html><body>";
	for (size_t i = 0; i < dir.size(); i++) {
		*res += "<br />Katalog: <a href='";
		*res += dir[i];
		*res += "'>";
		*res += dirName[i];
		*res += "</a>";
	}

	for (size_t i = 0; i < file.size(); i++) {
		*res += "<br />Plik: <a href='";
		*res += file[i];
		*res += "'>";
		*res += fileName[i];
		*res += "</a>";
	}
	*res += "</body></html>";
	return res;
}

}
