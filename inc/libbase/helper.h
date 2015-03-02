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


#elif MKL26Z4
#define LIBBASE_H(module) QUOTE(libbase/kl26/module.h)
#define LIBBASE_NS libbase::kl26
#define LIBBASE_MODULE(module) LIBBASE_NS::module

#endif
