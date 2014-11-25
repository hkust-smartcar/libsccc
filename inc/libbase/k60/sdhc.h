/*
 * sdhc.h
 *
 *  Created on: 23 Nov, 2014
 *      Author: harrison
 */
//#include "libbase/k60/hardware.h"
#include "libbase/k60/pin.h"
#include "libbase/log.h"
#ifndef SDHC_H_
#define SDHC_H_

namespace libbase {
namespace k60 {
enum CMDTYP{
	NORMAL,
	SUSPEND,
	RESUME,
	ABORT
};
enum RSPTYP{
	NORESPONSE = 0x0, //	No response
	RL136 = 0x1, //	Response length 136
	RL48 = 0x2, //	Response length 48
	RL48CHECKBUSY = 0x3, //	Response length 48, check busy after response
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
	/*CMD21~23 Reservered*/
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
enum CMDRESPONSE{
	NOMEANING = 0,
	TIMEOUT,
	ERROR,
	RESPONSERECEIVED
};
struct CMD{
	CMDINDEX cmdindex;
	CMDTYP cmdtyp; //CMD Type
	uint32_t dpsel; //Data Present Select
	uint32_t cicen; //Command Index Check Enable
	uint32_t cccen; //Command CRC Check Enable
	RSPTYP rsptyp; //Response Type Select
	uint32_t dtdsel; //Data Transfer Direction Select
	uint32_t msbsel; // multi-block transfer
	uint32_t bcen; //finite block number
	uint32_t ac12en; //if Auto CMD12 is to use
	uint32_t response[4];

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
	CMD constructCMD(CMDINDEX cmdindex);
	CMDRESPONSE sendCMD(CMD& cmd, uint32_t cmdArgs=0);

private:
	Pin mD1; //4-bit mode: DAT1 line or interrupt detect
	Pin mD0; //DAT0 line or busy-state detect
	Pin mClk; //Generated clock used to drive the MMC, SD, SDIO or CE-ATA cards.
	Pin mCmd; //Send commands to and receive responses from the card.
	Pin mD3; //4-/8-bit mode: DAT3 line or configured as card detection pin
	Pin mD2; //4-/8-bit mode: DAT2 line or read wait

	bool cardInserted;

};

} /* namespace k60 */
} /* namespace libbase */

#endif /* SDHC_H_ */
