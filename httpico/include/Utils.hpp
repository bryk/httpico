/**
 =========================================================================================
 File name	: /httpico/include/Utils.hpp
 Copyright	: Copyright © 2012 Piotr Bryk, Licensed under the Apache License, Version 2.0
 =========================================================================================
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

namespace Utils {

void initShouldExit();
bool shouldExit();
void setShouldExit();
void log(const char *fmt, ...);
void dbg(const char *fmt, ...);

}
#endif 
