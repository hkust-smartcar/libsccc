/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/reg_file.h"

#include "libsc/config.h"
#include "libsc/kl26/system.h"

using namespace libbase::kl26;

namespace libsc
{
namespace kl26
{

System *System::m_instance = nullptr;

System::System()
		: m_delay(),
		  m_timer(),
		  m_reg_file()
{}

}
}
