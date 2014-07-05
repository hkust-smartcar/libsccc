/*
 * i2c_utils.h
 * Utilities specified for K60
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_I2C_UTILS_H_
#define LIBSC_I2C_UTILS_H_

#include <FIRE_PORT_cfg.h>
#include <MK60_i2c.h>
#include <MK60_port.h>

namespace libsc
{

class I2cUtils
{
public:
	template<PTXn_e scl, PTXn_e sda>
	static constexpr inline I2Cn_e GetI2cChannel();
};

template<>
constexpr inline I2Cn_e I2cUtils::GetI2cChannel<I2C0_SCL, I2C0_SDA>()
{
	return I2C0;
}

template<>
constexpr inline I2Cn_e I2cUtils::GetI2cChannel<I2C1_SCL, I2C1_SDA>()
{
	return I2C1;
}

}

#endif /* LIBSC_I2C_UTILS_H_ */
