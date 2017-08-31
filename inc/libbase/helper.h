/*
 * helper.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define QUOTE(str) #str
#if MK60DZ10 || MK60D10 || MK60F15
#define LIBBASE_H(module) QUOTE(libbase/k60/module.h)
#define LIBBASE_NS libbase::k60
#define LIBBASE_MODULE(module) LIBBASE_NS::module
#define LIBSC_H(module) QUOTE(libsc/k60/module.h)
#define LIBSC_NS libsc::k60
#define LIBSC_MODULE(module) LIBSC_NS::module


#elif MKL26Z4
#define LIBBASE_H(module) QUOTE(libbase/kl26/module.h)
#define LIBBASE_NS libbase::kl26
#define LIBBASE_MODULE(module) LIBBASE_NS::module
#define LIBSC_H(module) QUOTE(libsc/kl26/module.h)
#define LIBSC_NS libsc::kl26
#define LIBSC_MODULE(module) LIBSC_NS::module

#endif
