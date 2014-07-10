/*
 * string_utils.cpp
 * String util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <memory>
#include <string>

#include "libutil/string.h"

using namespace std;

namespace libutil
{

string String::Format(const char *format, ...)
{
	va_list vl;
	va_start(vl, format);
	const string &product = Format(format, &vl);
	va_end(vl);
	return product;
}

string String::Format(const char *format, va_list *vl)
{
	static char product[64];
	vsnprintf(product, 64 * sizeof(char), format, *vl);
	product[63] = '\0';
	return product;
}

}
