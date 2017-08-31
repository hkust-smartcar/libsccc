/*
 * flash.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstring>
#include <cassert>
#include <stddef.h>
#include <libbase/k60/cmsis/mk60f15.h>

namespace libbase
{
namespace k60
{

class Flash
{

public:

	struct Config
	{
		/*
		 * count from last sector
		 * you can change it if you really know it
		 ** ONLY allow last five sectors to use
		*/
		uint8_t sectorStartIndex = 127;

		/* size that you want to use */
		/** DO NOt WASTE MEMORY PLZ **/
		size_t size = 0x1000;
	};

	enum FlashStatus
	{
		kSuccess			= 0,
		kCommandError		= FTFE_FSTAT_MGSTAT0_MASK,
		// input size bigger than the size wanted while writing
		kSizeExceeded		= 0x2u,
		kParametersInvalid	= 0x4u,
		kReserved			= 0x8u,
		kProtectViolation	= FTFE_FSTAT_FPVIOL_MASK,
		kAccessDenied		= FTFE_FSTAT_ACCERR_MASK,
		kReadCollisionError = FTFE_FSTAT_RDCOLERR_MASK
	};

	/* 512KB Data Flash - 4KB sector */
	static const uint16_t 		MaxSectorsCount = 128;
	static const uint16_t 		DefaultSectorSize = 0x1000;
	static const uint16_t		MinSectorIndex = 124;

	Flash(Config config);
	FlashStatus Read(void *outBytes, size_t sizeOfBytes);
	FlashStatus Write(void *inBytes, size_t sizeOfBytes);
	uint32_t GetStartAddr(void);

private:

	enum struct FlashCmd
	{
		kProhram4Byte = 0x06, // FTFE not supported
		kProgram8Byte,
		kEraseBlock,
		kEraseSector
	};

	uint32_t				m_startAddr;
	size_t					m_maxSize;

	inline void WaitForFlashCmdComplete(void);
	FlashStatus ExecuteCmd(void);

	FlashStatus Program(const uint32_t FlashTargetAddr, void *source, size_t sizeOfBytes);

protected:

	FlashStatus EraseSector(const uint32_t FlashTargetAddr);

};

}
}
