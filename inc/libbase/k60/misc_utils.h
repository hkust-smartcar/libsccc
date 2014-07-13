/*
 * misc_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_MISC_UTILS_H_
#define LIBBASE_MISC_UTILS_H_

#include <cstdint>

#define SET_BIT(x, n) ((x) |= 1 << (n))
#define RESET_BIT(x, n) ((x) &= ~(1 << (n)))
#define CLEAR_BIT(x, n) RESET_BIT(x, n)
#define GET_BIT(x, n) (((x) >> (n)) & 1)

typedef unsigned int Uint;
typedef unsigned char Byte;

// host to big-endian encoding
uint32_t htobe32(const uint32_t host_32bits);
// host to lil-endian encoding
uint32_t htole32(const uint32_t host_32bits);
// big-endian to host encoding
uint32_t be32toh(const uint32_t big_endian_32bits);
// lil-endian to host encoding
uint32_t le32toh(const uint32_t little_endian_32bits);

#endif /* LIBBASE_MISC_UTILS_H_ */
