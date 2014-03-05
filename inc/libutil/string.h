/*
 * string_utils.h
 * String util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_STRING_UTILS_H_
#define LIBUTIL_STRING_UTILS_H_

#include <cstdarg>

#include <memory>
#include <string>

namespace libsc
{

class StringUtils
{
public:
	static std::string Format(const char *format, ...);
	static std::string Format(const char *format, va_list *vl);
};

}

#endif /* LIBUTIL_STRING_UTILS_H_ */
