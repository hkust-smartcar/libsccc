/*
 * flash.cpp
 *
 * Author: Peter Lau
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/flash.h"

#ifdef MK60F15_H_

Flash::Flash(Config config)
:
	m_startAddr(0),
	m_maxSize(0)
{
	if (config.sectorStartIndex < MinSectorIndex &&
		config.sectorStartIndex + config.size / 0x800 > MaxSectorsCount)
		assert(false);

	m_startAddr = config.sectorStartIndex * 0x800;
	m_maxSize = config.size;
}

Flash::FlashStatus Flash::Read(void *outBytes, size_t sizeOfBytes)
{
	if (sizeOfBytes == 0 || sizeOfBytes > m_maxSize)
		return FlashStatus::kParametersInvalid;

	memcpy(outBytes, (void *)m_startAddr, sizeOfBytes);

	return FlashStatus::kSuccess;
}

Flash::FlashStatus Flash::Write(void *inBytes, size_t sizeOfBytes)
{
	FlashStatus ret = FlashStatus::kSuccess;

	ret = EraseSector(m_startAddr);
	if (ret != FlashStatus::kSuccess)
		return ret;

	return Program(m_startAddr, inBytes, sizeOfBytes);
}

uint32_t Flash::GetStartAddr(void)
{
	return m_startAddr;
}

inline void Flash::WaitForFlashCmdComplete(void)
{
	while (!(FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK));
}

Flash::FlashStatus Flash::ExecuteCmd(void)
{
	uint8_t ret = (uint8_t)FlashStatus::kSuccess;

	FTFE->FSTAT |= FTFE_FSTAT_CCIF_MASK;
	WaitForFlashCmdComplete();

	if (FTFE->FSTAT & FTFE_FSTAT_MGSTAT0_MASK)
		ret |= FTFE_FSTAT_MGSTAT0_MASK;
	else if (FTFE->FSTAT & FTFE_FSTAT_FPVIOL_MASK)
	{
		FTFE->FSTAT |= FTFE_FSTAT_FPVIOL_MASK;
		ret |= FTFE_FSTAT_FPVIOL_MASK;
	}
	else if (FTFE->FSTAT & FTFE_FSTAT_ACCERR_MASK)
	{
		FTFE->FSTAT |= FTFE_FSTAT_ACCERR_MASK;
		ret |= FTFE_FSTAT_ACCERR_MASK;
	}
	else if (FTFE->FSTAT & FTFE_FSTAT_RDCOLERR_MASK)
	{
		FTFE->FSTAT |= FTFE_FSTAT_RDCOLERR_MASK;
		ret |= FTFE_FSTAT_RDCOLERR_MASK;
	}

	return (Flash::FlashStatus)ret;
}

Flash::FlashStatus Flash::EraseSector(const uint32_t FlashTargetAddr)
{
	WaitForFlashCmdComplete();

	// Command
	FTFE->FCCOB0 = (uint8_t)FlashCmd::kEraseSector;
	// Address
	FTFE->FCCOB3 = (uint8_t)(FlashTargetAddr & 0xFF);
	FTFE->FCCOB2 = (uint8_t)((FlashTargetAddr >> 8) & 0xFF);
	FTFE->FCCOB1 = (uint8_t)(FlashTargetAddr >> 16);

	return ExecuteCmd();
}

Flash::FlashStatus Flash::Program(const uint32_t FlashTargetAddr, void *source, size_t sizeOfBytes)
{
	FlashStatus ret = FlashStatus::kSuccess;

	if (sizeOfBytes == 0 || sizeOfBytes > m_maxSize)
		return FlashStatus::kParametersInvalid;

	uint32_t tempTargetAddr = FlashTargetAddr;
	uint8_t *tempSrcAddr = (uint8_t *)source;

	WaitForFlashCmdComplete();

	uint8_t temp[8];
	uint8_t i = 0;
	while (sizeOfBytes && ret == FlashStatus::kSuccess)
	{
		// Command
		FTFE->FCCOB0 = (uint8_t)FlashCmd::kProgram8Byte;

		// Address
		FTFE->FCCOB3 = (uint8_t)(tempTargetAddr & 0xFF);
		FTFE->FCCOB2 = (uint8_t)((tempTargetAddr >> 8) & 0xFF);
		FTFE->FCCOB1 = (uint8_t)(tempTargetAddr >> 16);

		// Data
		memset(temp, 0xFF, 8);

		for (i = 0; i < 8 && sizeOfBytes; sizeOfBytes--, i++)
			temp[i] = *((uint8_t *)tempSrcAddr + i);
		tempSrcAddr += i;
		tempTargetAddr += i;

		FTFE->FCCOB7 = (uint8_t)(*(temp + 0));
		FTFE->FCCOB6 = (uint8_t)(*(temp + 1));
		FTFE->FCCOB5 = (uint8_t)(*(temp + 2));
		FTFE->FCCOB4 = (uint8_t)(*(temp + 3));
		FTFE->FCCOBB = (uint8_t)(*(temp + 4));
		FTFE->FCCOBA = (uint8_t)(*(temp + 5));
		FTFE->FCCOB9 = (uint8_t)(*(temp + 6));
		FTFE->FCCOB8 = (uint8_t)(*(temp + 7));

		ret = ExecuteCmd();
	}

	return ret;
}

#endif
