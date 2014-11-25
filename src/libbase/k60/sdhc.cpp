/*
 * sdhc.cpp
 *
 *  Created on: 23 Nov, 2014
 *      Author: harrison
 */
#include <cstdint>
#include <cassert>
#include <bitset>
#include "libbase/k60/hardware.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/sdhc.h"
#include "libbase/misc_utils_c.h"
#include "libbase/k60/pin.h"
#include "libsc/k60/system.h"
#include "libbase/k60/mcg.h"

namespace libbase {
namespace k60 {
SDHC_Type* MEM_MAP = SDHC;

Pin::Config sdhc::getD1Config(){
	Pin::Config d1_config;
	d1_config.pin = Pin::Name::kPte0;
	d1_config.mux = Pin::Config::MuxControl::kAlt4;
	d1_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	d1_config.config.set(Pin::Config::ConfigBit::kPullEnable);
	d1_config.config.set(Pin::Config::ConfigBit::kPullUp);
	return d1_config;
}

Pin::Config sdhc::getD0Config(){
	Pin::Config d0_config;
	d0_config.pin = Pin::Name::kPte1;
	d0_config.mux = Pin::Config::MuxControl::kAlt4;
	d0_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	d0_config.config.set(Pin::Config::ConfigBit::kPullEnable);
	d0_config.config.set(Pin::Config::ConfigBit::kPullUp);
	return d0_config;
}

Pin::Config sdhc::getClkConfig(){
	Pin::Config clk_config;
	clk_config.pin = Pin::Name::kPte2;
	clk_config.mux = Pin::Config::MuxControl::kAlt4;
	clk_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	return clk_config;
}

Pin::Config sdhc::getCmdConfig(){
	Pin::Config cmd_config;
	cmd_config.pin = Pin::Name::kPte3;
	cmd_config.mux = Pin::Config::MuxControl::kAlt4;
	cmd_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	cmd_config.config.set(Pin::Config::ConfigBit::kPullEnable);
	cmd_config.config.set(Pin::Config::ConfigBit::kPullUp);
	return cmd_config;
}

Pin::Config sdhc::getD3Config(){
	Pin::Config d3_config;
	d3_config.pin = Pin::Name::kPte4;
	d3_config.mux = Pin::Config::MuxControl::kAlt4;
	d3_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	d3_config.config.set(Pin::Config::ConfigBit::kPullEnable);
	d3_config.config.set(Pin::Config::ConfigBit::kPullUp);
	return d3_config;
}

Pin::Config sdhc::getD2Config(){
	Pin::Config d2_config;
	d2_config.pin = Pin::Name::kPte5;
	d2_config.mux = Pin::Config::MuxControl::kAlt4;
	d2_config.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
	d2_config.config.set(Pin::Config::ConfigBit::kPullEnable);
	d2_config.config.set(Pin::Config::ConfigBit::kPullUp);
	return d2_config;
}

void sdhc::SDHC_set_baudrate(uint32_t baudrate)
{
	uint32_t clock = Mcg::Get().GetCoreClock();
	uint32_t pres, div, min, minpres = 0x80, mindiv = 0x0F;
	int32_t  val;

    /* Find closest setting */
    min = (uint32_t)-1;
    for (pres = 2; pres <= 256; pres <<= 1)
    {
        for (div = 1; div <= 16; div++)
        {
            val = pres * div * baudrate - clock;
            if (val >= 0)
            {
                if (min > val)
                {
                    min = val;
                    minpres = pres;
                    mindiv = div;
                }
            }
        }
    }

    /* Disable ESDHC clocks */
    MEM_MAP->SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);

    /* Change dividers */
    div = MEM_MAP->SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK));
    MEM_MAP->SYSCTL = div | (SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_SDCLKFS(minpres >> 1) | SDHC_SYSCTL_DVS(mindiv - 1));

    /* Wait for stable clock */
    while (0 == (MEM_MAP->PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK))
    {
        /* Workaround... */
    	libsc::k60::System::DelayMs(10);
    };

    /* Enable ESDHC clocks */
    MEM_MAP->SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
    MEM_MAP->IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
}


sdhc::sdhc() {
	cardInserted = false;

	/*Enable SDHC Clock*/
	Sim::SetEnableClockGate(Sim::ClockGate::kSdhc, true);
	/*Software Reset For ALL*/
	SET_BIT(MEM_MAP->SYSCTL,SDHC_SYSCTL_RSTA_SHIFT);

	SDHC_set_baudrate(400000);

	/*Init All Pins - Open Drain Mode to avoid Bus contention*/
	mD1 = Pin(getD1Config());
	mD0 = Pin(getD0Config());
	mClk = Pin(getClkConfig());
	mCmd = Pin(getCmdConfig());
	mD3 = Pin(getD3Config());
	mD2 = Pin(getD2Config());

	/* Enable IRQ requests */
	MEM_MAP->IRQSTAT = 0xFFFF;
	MEM_MAP->IRQSTATEN = 	  SDHC_IRQSTATEN_DEBESEN_MASK | SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK
			| SDHC_IRQSTATEN_CIESEN_MASK | SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK
			| SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK
			| SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;

	/*System Control Register - Send 80 Clocks to SDHC*/
	SET_BIT(MEM_MAP->SYSCTL, SDHC_SYSCTL_INITA_SHIFT);

	/*Send broadcast command CMD3, then enter standby mode.*/
	CMD cmd = constructCMD(CMD3);
	sendCMD(cmd);
	/*Addressed type commands are used from this point.
	 * In this mode, the CMD/DAT I/O pads will turn to push-pull mode*/
//	Reinit causing hard fault
//	Pin::Config cfg = getD1Config();
//	mD1 = Pin(cfg);
//
//	cfg = getD0Config();
//	mD0 = Pin(cfg);
//
//	cfg = getClkConfig();
//	mClk = Pin(getClkConfig());
//
//	cfg = getCmdConfig();
//	mCmd = Pin(getCmdConfig());
//
//	cfg = getD3Config();
//	mD3 = Pin(cfg);
//
//	cfg = getD2Config();
//	mD2 = Pin(cfg);
	constructCMD(CMD0);
	sendCMD(cmd);
}

sdhc::~sdhc() {
}

bool sdhc::cardDetect(){
	/*Set the CINSIEN bit to enable card detection interrupt*/
	SET_BIT(MEM_MAP->IRQSIGEN,SDHC_IRQSIGEN_CINSIEN_SHIFT);
	/*When an interrupt from the SDHC is received, check the IRQSTAT[CINS] bit in the
Interrupt Status register to see if it was caused by card insertion*/
	if(GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CINS_SHIFT)){
		cardInserted = true;
		/*Clear the CINSIEN bit to disable the card detection interrupt and ignore all card
			insertion interrupts afterwards*/
		RESET_BIT(MEM_MAP->IRQSIGEN,SDHC_IRQSIGEN_CINSIEN_SHIFT);
	}
	return cardInserted;
}

void sdhc::cardReset(){
	/*write “1” to RSTA bit to reset SDHC*/
	SET_BIT(MEM_MAP->SYSCTL,SDHC_SYSCTL_RSTA_SHIFT);
	/*set DTOCV and SDCLKFS bit fields to get the SD_CLK of frequency around 400kHz*/
	SET_BIT(MEM_MAP->SYSCTL,SDHC_SYSCTL_DTOCV_SHIFT);
	SET_BIT(MEM_MAP->SYSCTL,SDHC_SYSCTL_SDCLKFS_SHIFT);
	/*configure IO pad to set the power voltage of external card to around 3.0V*/

	/*poll bits CIHB and CDIHB bits of PRSSTAT to wait both bits are cleared*/
	while(GET_BIT(MEM_MAP->PRSSTAT, SDHC_PRSSTAT_CIHB_SHIFT) | GET_BIT(MEM_MAP->PRSSTAT, SDHC_PRSSTAT_CDIHB_SHIFT));
	/*Send 80 clocks to card*/
	SET_BIT(MEM_MAP->SYSCTL, SDHC_SYSCTL_INITA_SHIFT);
	/*send CMD0/CMD52 to card to reset card*/
	CMD cmd = constructCMD(CMD0);
	sendCMD(cmd);
	/*V oltage V alidation*/
}

CMD sdhc::constructCMD(CMDINDEX cmdindex){
	CMD cmd;
	/*CMD {cmdindex, CMDTYP, DPSEL, CICEN, CCCEN,  RSPTYP, DTDSEL, MSBSEL, BCEN, AC12EN};*/
	switch(cmdindex){
	case CMD0:
	case CMD1:
	case CMD4:
	case CMD15:
		cmd = {cmdindex, NORMAL, 0, 0, 0, NORESPONSE, 0,0,0,0,0};
		break;
	case CMD2:
	case CMD9:
	case CMD10:
		cmd = {cmdindex, NORMAL, 0, 0, 1, RL136, 0,0,0,0};
		break;
	case CMD3:
	case CMD6:
	case CMD8:
	case CMD11:
	case CMD13:
	case CMD16:
	case CMD17:
	case CMD20:
		//case ACMD22:
		//case ACMD23:
	case CMD24:
	case CMD26:
	case CMD27:
	case CMD30:
	case CMD32:
	case CMD33:
	case CMD34:
	case CMD35:
	case CMD36:
	case CMD37:
	case CMD40:
		//case ACMD51:
	case CMD52:
	case CMD53:
	case CMD55:
		cmd = {cmdindex, NORMAL, 0, 1, 1, RL48, 0,0,0,0};
		break;
	case CMD5:
	case CMD39:
		//case ACMD41:
		cmd = {cmdindex, NORMAL, 0, 0, 0, RL48, 0,0,0,0};
		break;
	case CMD7:
	case CMD28:
	case CMD29:
	case CMD38:
	case CMD42:
	case CMD56:
	case CMD60:
	case CMD61:
		cmd = {cmdindex, NORMAL, 0, 1, 1, RL48CHECKBUSY, 0, 0,0,0};
		break;
	case CMD12:
		cmd = {cmdindex, ABORT, 0, 1, 1, RL48CHECKBUSY, 0, 0,0,0};
		break;
	case CMD18:
	case CMD25:
		cmd = {cmdindex, NORMAL, 0, 1, 1, RL48, 0,0,0,1};
		break;
	default:
		break;
	}
	return cmd;
}

CMDRESPONSE sdhc::sendCMD(CMD& cmd, uint32_t cmdArgs)
{
	CMDRESPONSE res = RESPONSERECEIVED;
	/*WORD wCmd; // 32-bit integer to make up the data to write into Transfer Type register, it is recommended to implement in a bit-field manner*/
	uint32_t wCmd = SDHC_XFERTYP_CMDINX(cmd.cmdindex);

	/*set CMDTYP, DPSEL, CICEN, CCCEN, RSTTYP, DTDSEL accorind to the command index;*/
	wCmd |= SDHC_XFERTYP_CMDTYP(cmd.cmdtyp);
	if(cmd.dpsel) SET_BIT(wCmd, SDHC_XFERTYP_DPSEL_SHIFT);
	if(cmd.cicen) SET_BIT(wCmd, SDHC_XFERTYP_CICEN_SHIFT);
	if(cmd.cccen) SET_BIT(wCmd, SDHC_XFERTYP_CCCEN_SHIFT);
	if(cmd.rsptyp) wCmd |= SDHC_XFERTYP_RSPTYP(cmd.rsptyp);
	if(cmd.dtdsel) SET_BIT(wCmd, SDHC_XFERTYP_DTDSEL_SHIFT);

	//	if (internal DMA is used) wCmd |= 0x1; //internal dma not implemented
	//Check multi-block transfer
	if (cmd.msbsel) {
		SET_BIT(wCmd, SDHC_XFERTYP_MSBSEL_SHIFT);
		if (cmd.bcen) {
			SET_BIT(wCmd, SDHC_XFERTYP_BCEN_SHIFT);
			if (cmd.ac12en) SET_BIT(wCmd, SDHC_XFERTYP_AC12EN_SHIFT);
		}
	}
	//make sure PRSSTAT[CDIHB] bit is NOT set, so that CMDARG is not write protected.
	while (GET_BIT(MEM_MAP->PRSSTAT, SDHC_PRSSTAT_CIHB_SHIFT) || GET_BIT(MEM_MAP->PRSSTAT, SDHC_PRSSTAT_CDIHB_SHIFT)) {};
	// configure the command argument
	MEM_MAP->CMDARG = cmdArgs;
	// set Transfer Type register as wCmd value to issue the command
	MEM_MAP->XFERTYP = wCmd;


	//wait_for_response(cmd_index)
	while((MEM_MAP->IRQSTAT & (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK)) == 0);

	// wait until Command Complete bit is set
//	while(~GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CC_SHIFT));

	//Check if any error bits about Command are set at complete
	if((MEM_MAP->IRQSTAT & (SDHC_IRQSTAT_CC_MASK | SDHC_IRQSTAT_AC12E_MASK | SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CTOE_MASK)) != SDHC_IRQSTAT_CC_MASK){
		assert("Error");
		res = ERROR;
	}
	if(~GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CC_SHIFT) && ~GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CTOE_SHIFT) ){
		//Response No meaning
		assert("x");
		res = NOMEANING;
	}

	if(GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CTOE_SHIFT) ){
		//	Response not received within 64 SDCLK cycles
//		MEM_MAP->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
		assert("Timeout - Response not received within 64 SDCLK cycles");
		MEM_MAP->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK;
		res = TIMEOUT;
	}

	if(GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CC_SHIFT) && ~GET_BIT(MEM_MAP->IRQSTAT, SDHC_IRQSTAT_CTOE_SHIFT) ){
		//	Response received
		LOG_D("Response received\n");
		cmd.response[0] = MEM_MAP->CMDRSP[0];
		cmd.response[1] = MEM_MAP->CMDRSP[1];
		cmd.response[2] = MEM_MAP->CMDRSP[2];
		cmd.response[3] = MEM_MAP->CMDRSP[3];
		res = RESPONSERECEIVED;
	}

	//write 1 to clear CC bit and all Command Error bits;
	MEM_MAP->IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

	return res;

}

} /* namespace k60 */
} /* namespace libbase */
