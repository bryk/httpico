/**
 =========================================================================================
 File name	: /httpico/include/ConfigurationReader.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef CONFIGURATIONREADER_HPP_
#define CONFIGURATIONREADER_HPP_
#include <cstdio>
#include "ForwardDeclarations.hpp"

namespace Httpico {

/*
 * Httpico::ConfigurationReader
 */
class ConfigurationReader {
public:
	ConfigurationReader(FILE *f);
	bool fillConfig(HttpServerConfiguration &conf);
	virtual ~ConfigurationReader();

private:
	FILE *file;
};

} //namespace Httpico
#endif 
