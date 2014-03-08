/*
 * spi_utils.h
 * Utilities specified for K60
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_SPI_UTILS_H_
#define LIBSC_SPI_UTILS_H_

#include <FIRE_PORT_cfg.h>
#include <MK60_port.h>
#include <MK60_spi.h>

namespace libsc
{

class SpiUtils
{
public:
	/**
	 * Return the SPI module set for the specified GPIO pins @a sck, @a miso,
	 * @a mosi and @a ss. This method will cause a link-time error if such
	 * combination of pins is unknown
	 *
	 * @return
	 */
	template<PTXn_e sck, PTXn_e mosi, PTXn_e miso, PTXn_e ss>
	static inline SPIn_e GetSpiModule();

	/**
	 * Return the SPI PCS set for the specified GPIO pins @a sck, @a miso,
	 * @a mosi and @a ss. This method will cause a link-time error if such
	 * combination of pins is unknown
	 *
	 * @return
	 */
	template<PTXn_e sck, PTXn_e mosi, PTXn_e miso, PTXn_e ss>
	static inline SPIn_PCSn_e GetSpiPcs();
};

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS0>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS1>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS2>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS3>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS4>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS5>()
{
	return SPI0;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS0>()
{
	return SPI1;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS1>()
{
	return SPI1;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS2>()
{
	return SPI1;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS3>()
{
	return SPI1;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI2_SCK, SPI2_SOUT, SPI2_SIN, SPI2_PCS0>()
{
	return SPI2;
}

template<>
inline SPIn_e SpiUtils::GetSpiModule<SPI2_SCK, SPI2_SOUT, SPI2_SIN, SPI2_PCS1>()
{
	return SPI2;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS0>()
{
	return SPIn_PCS0;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS1>()
{
	return SPIn_PCS1;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS2>()
{
	return SPIn_PCS2;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS3>()
{
	return SPIn_PCS3;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS4>()
{
	return SPIn_PCS4;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI0_SCK, SPI0_SOUT, SPI0_SIN, SPI0_PCS5>()
{
	return SPIn_PCS5;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS0>()
{
	return SPIn_PCS0;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS1>()
{
	return SPIn_PCS1;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS2>()
{
	return SPIn_PCS2;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI1_SCK, SPI1_SOUT, SPI1_SIN, SPI1_PCS3>()
{
	return SPIn_PCS3;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI2_SCK, SPI2_SOUT, SPI2_SIN, SPI2_PCS0>()
{
	return SPIn_PCS0;
}

template<>
inline SPIn_PCSn_e SpiUtils::GetSpiPcs<SPI2_SCK, SPI2_SOUT, SPI2_SIN, SPI2_PCS1>()
{
	return SPIn_PCS1;
}

}

#endif /* LIBSC_SPI_UTILS_H_ */
