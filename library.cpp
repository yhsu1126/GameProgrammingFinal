#include "stdafx.h"
#ifndef FLY2
#define FLY2
#include "library.h"
#endif

void strtoChar(char *buf, string str) {
	strncpy(buf, str.c_str(), sizeof(buf));
	buf[sizeof(buf) - 1] = 0;
}