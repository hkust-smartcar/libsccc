/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/cache.h"

#include "libsc/config.h"
#include "libsc/k60/system.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

System *System::m_instance = nullptr;

System::System()
		: m_delay(),
		  m_timer()
{
	// Enable cache unless otherwise disabled
#if !LIBSC_NOT_USE_CACHE && MK60F15
	Cache::Config cache_conf;
	cache_conf.is_enable = true;
	Cache::Get().Init(cache_conf);
#endif
}

}
}
