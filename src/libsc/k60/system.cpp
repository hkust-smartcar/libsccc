/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

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
{}

}
}
