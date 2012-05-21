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
}

Buffer * DirectoryResponseProcessor::getContent() throw (std::exception) {
	Utils::dbg("DIRECTORY\n");
	struct stat buf;
	DIR *dp = opendir(request.reqestedResourcePath.c_str());
	if (!dp) {
		perror(request.reqestedResourcePath.c_str());
		throw std::exception();
	}
	std::vector<std::string> file;
	std::vector<std::string> fileName;
	std::vector<std::string> dir;
	std::vector<std::string> dirName;
	dir.push_back(request.reqestedResourcePath.substr(configuration.getServerRoot().size()) + "/..");
	dirName.push_back("..");

	struct dirent * ent;
	errno = 0;
	while ((ent = readdir(dp)) != NULL) {
		if (errno) {
			perror((request.reqestedResourcePath + " readdir").c_str());
			errno = 0;
			continue;
		}
		std::string path = ent->d_name;
		if (path == "." || path == "..") {
			continue;
		} else {
			path = request.reqestedResourcePath + "/" + path;
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
		perror(request.reqestedResourcePath.c_str());
		errno = 0;
	}
	std::string relative = request.reqestedResourcePath.substr(configuration.getServerRoot().size());
	if (relative.size() == 0) {
		relative = "/";
	}
	std::string title = "Index of " + relative;
	std::string content = "<ul>";

	for (size_t i = 0; i < dir.size(); i++) {
		content += "<li class=\"folder\"><a href=\"";
		content += Utils::urlEncode(dir[i]);
		content += "\">";
		content += dirName[i];
		content += "</a></li>";
	}

	for (size_t i = 0; i < file.size(); i++) {
		content += "<li class=\"file\"><a href=\"";
		content += Utils::urlEncode(file[i]);
		content += "\">";
		content += fileName[i];
		content += "</a></li>";
	}
	content += "</ul>";
	Utils::dbg("Po DIRectory\n");
	return Utils::getTempatedHtmlFile(title, content, configuration);
}

}
