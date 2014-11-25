/*
 * sdhc.h
 *
 *  Created on: 23 Nov, 2014
 *      Author: harrison
 */
//#include "libbase/k60/hardware.h"
#include "libbase/k60/pin.h"

#ifndef SDHC_H_
#define SDHC_H_

namespace libbase {
namespace k60 {
enum CMDTYPE{
	bc, /*Broadcast command type*/
	bcr, /*Broadcast command type*/
	ac, /*Address command type*/
	adtc /*Address command type*/
};
enum CMDINDEX{
	CMD0 = 0,
	CMD1,
	CMD2,
	CMD3,
	CMD4,
	CMD5,
	CMD6,
	CMD7,
	CMD8,
	CMD9,
	CMD10,
	CMD11,
	CMD12,
	CMD13,
	CMD14,
	CMD15,
	CMD16,
	CMD17,
	CMD18,
	CMD19,
	CMD20,
	/*CMD21, 22, 23 Reservered*/
	CMD24 = 24,
	CMD25,
	CMD26,
	CMD27,
	CMD28,
	CMD29,
	CMD30,
	/*CMD31 Reserved*/
	CMD32,
	CMD33,
	CMD34,
	CMD35,
	CMD36,
	CMD37,
	CMD38,
	CMD39,
	CMD40,
	/*CMD41 Reserved*/
	CMD42 = 42,
	/*CMD43~51 Reserved*/
	CMD52 = 52,
	CMD53,
	/*CMD54 Reserved*/
	CMD55 = 55,
	CMD56,
	/*CMD57~59 Reserved*/
	CMD60 = 60,
	CMD61,
	/*CMD62~63 Reserved*/
};
struct CMD{
	CMDINDEX cmdindex;
	uint32_t CMDTYP;
	uint32_t DPSEL;
	uint32_t CICEN;
	uint32_t CCCEN;
	uint32_t RSPTYP;
	uint32_t DTDSEL;
	bool MBT; // multi-block transfer
	bool FBN; //finite block number
	bool AC12EN; //if cauto12 command is to use

};
class sdhc {
public:
	Pin::Config getD1Config();
	Pin::Config getD0Config();
	Pin::Config getClkConfig();
	Pin::Config getCmdConfig();
	Pin::Config getD3Config();
	Pin::Config getD2Config();

	sdhc();
	virtual ~sdhc();
	bool cardDetect();
	void cardReset();
	uint32_t sendCMD(CMDINDEX, uint32_t cmdArgs = 0);

private:
	Pin d1; //4-bit mode: DAT1 line or interrupt detect
	Pin d0; //DAT0 line or busy-state detect
	Pin clk; //Generated clock used to drive the MMC, SD, SDIO or CE-ATA cards.
	Pin cmd; //Send commands to and receive responses from the card.
	Pin d3; //4-/8-bit mode: DAT3 line or configured as card detection pin
	Pin d2; //4-/8-bit mode: DAT2 line or read wait

	bool cardInserted;

};

} /* namespace k60 */
} /* namespace libbase */

#endif /* SDHC_H_ */
