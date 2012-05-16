/**
 =========================================================================================
 File name	: httpico.cpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#include <cstdio>
#include <cstdlib>
#include "HttpRequest.hpp"
#include "HttpServer.hpp"
#include "HttpServerConfiguration.hpp"

using namespace std;
using namespace Httpico;

int main(void) {
	HttpServerConfiguration conf;
	HttpServer server(conf);
	server.start();
	return EXIT_SUCCESS;
}
