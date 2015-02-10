/*
 * misc_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/misc_utils_c.h"

// host to big-endian encoding
uint32_t htobe32(const uint32_t host_32bits);
// host to lil-endian encoding
uint32_t htole32(const uint32_t host_32bits);
// big-endian to host encoding
uint32_t be32toh(const uint32_t big_endian_32bits);
// lil-endian to host encoding
uint32_t le32toh(const uint32_t little_endian_32bits);

#define VOID
#define STATE_GUARD(clz, ret) if (!(*this)) { LOG_W(#clz " in illegal state @%d", __LINE__); return ret; }
