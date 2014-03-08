/*
 * string.h
 * String utilities
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_STRING_H_
#define LIBUTIL_STRING_H_

#include <cstdarg>

#include <memory>
#include <string>

namespace libutil
{

class String
{
public:
	static std::string Format(const char *format, ...);
	static std::string Format(const char *format, va_list *vl);
};

}

#endif /* LIBUTIL_STRING_UTILS_H_ */
