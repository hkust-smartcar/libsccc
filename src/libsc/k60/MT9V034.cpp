/*
 * MT9V034.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <functional>
#include <memory>

#include "libbase/log.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/dma_manager.h"
#include "libbase/k60/dma_mux.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/vectors.h"

#include "libsc/config.h"
#include "libsc/k60/MT9V034.h"
#include "libsc/system.h"
#include "libutil/misc.h"

using namespace libbase::k60;
using namespace libutil;
using namespace std;

namespace libsc {
namespace k60 {
#ifdef LIBSC_USE_MT9V034

namespace {
	SoftSccbMaster::Config GetSccbConfig() {
		SoftSccbMaster::Config product;
		product.sio_c_pin = LIBSC_MT9V034_SCL;
		product.sio_d_pin = LIBSC_MT9V034_SDA;
		return product;
	}

	GpiArray::Config GetGpiArrayConfig() {
		GpiArray::Config product;
		product.start_pin = LIBSC_MT9V034_DATA0;
		product.count = 8;
		product.config.set(Pin::Config::ConfigBit::kPullEnable);
		return product;
	}

	Gpi::Config GetClockConfig() {
		Gpi::Config product;
		product.pin = LIBSC_MT9V034_PCLK;
		product.config.set(Pin::Config::ConfigBit::kPullEnable);
		product.interrupt = Pin::Config::Interrupt::kDmaRising;
		return product;
	}

	Gpi::Config GetVsyncConfig(Gpi::OnGpiEventListener isr) {
		Gpi::Config product;
		product.pin = LIBSC_MT9V034_VSYNC;
		product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
		product.config.set(Pin::Config::ConfigBit::kPullEnable);
		product.config.set(Pin::Config::ConfigBit::kPullUp);
		product.interrupt = Pin::Config::Interrupt::kFalling;
		product.isr = isr;
		return product;
	}

}

void MT9V034::RegSet(uint8_t reg_addr, uint16_t value) {
	m_sccb.SendByte(0xB8 >> 1, reg_addr, value>>8);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, value&0b11111111);
}

MT9V034::MT9V034(const Config &config) :
m_sccb(GetSccbConfig()), m_data_array(GetGpiArrayConfig()), m_clock(GetClockConfig()), m_vsync(nullptr), m_dma(nullptr), m_w(1+752 / ((uint8_t) config.w_binning * 2)), m_h(480 / ((uint8_t) config.h_binning * 2)), m_buf_size(m_w * m_h), m_is_shoot(false), m_is_lock_buffer(false), m_is_available(false), m_is_dma_start(false) {
	Byte out_byte_high;
	Byte out_byte_low;
	uint16_t result;

//Verify the camera
	assert(m_sccb.GetByte(0xB8 >> 1, 0x00, &out_byte_high));
	assert(m_sccb.GetByte(0xB8 >> 1, 0xF0, &out_byte_low));
	result = out_byte_high;
	result <<= 8;
	result += out_byte_low;
	assert(result == 0x1324);

//Reset control circuit
	RegSet(0x0C,1);
	RegSet(0x0C,0);

//Load default
	RegSet(0x01, 0x0001);//COL_WINDOW_START_CONTEXTA_REG
	RegSet(0x02, 0x0004);//ROW_WINDOW_START_CONTEXTA_REG
	RegSet(0x03, 0x01E0);//ROW_WINDOW_SIZE_CONTEXTA_REG
	RegSet(0x04, 0x02F0);//COL_WINDOW_SIZE_CONTEXTA_REG
	RegSet(0x05, 0x005E);//HORZ_BLANK_CONTEXTA_REG
	RegSet(0x06, 0x0039);//VERT_BLANK_CONTEXTA_REG
	RegSet(0x07, 0x0188);//CONTROL_MODE_REG
	RegSet(0x08, 0x0190);//COARSE_SHUTTER_WIDTH_1_CONTEXTA
	RegSet(0x09, 0x01BD);//COARSE_SHUTTER_WIDTH_2_CONTEXTA
	RegSet(0x0A, 0x0164);//SHUTTER_WIDTH_CONTROL_CONTEXTA
	RegSet(0x0B, 0x01C2);//COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
	RegSet(0x0C, 0x0000);//RESET_REG
	RegSet(0x0D, 0x0300);//READ_MODE_REG
	RegSet(0x0E, 0x0000);//READ_MODE2_REG
	RegSet(0x0F, 0x0100);//PIXEL_OPERATION_MODE
	RegSet(0x10, 0x0040);//RAMP_START_DELAY
	RegSet(0x11, 0x8042);//OFFSET_CONTROL
	RegSet(0x12, 0x0022);//AMP_RESET_BAR_CONTROL
	RegSet(0x13, 0x2D2E);//5T_PIXEL_RESET_CONTROL
	RegSet(0x14, 0x0E02);//4T_PIXEL_RESET_CONTROL
	RegSet(0x15, 0x0E32);//TX_CONTROL
	RegSet(0x16, 0x2802);//5T_PIXEL_SHS_CONTROL
	RegSet(0x17, 0x3E38);//4T_PIXEL_SHS_CONTROL
	RegSet(0x18, 0x3E38);//5T_PIXEL_SHR_CONTROL
	RegSet(0x19, 0x2802);//4T_PIXEL_SHR_CONTROL
	RegSet(0x1A, 0x0428);//COMPARATOR_RESET_CONTROL
	RegSet(0x1B, 0x0000);//LED_OUT_CONTROL
	RegSet(0x1C, 0x0302);//DATA_COMPRESSION
	RegSet(0x1D, 0x0040);//ANALOG_TEST_CONTROL
	RegSet(0x1E, 0x0000);//SRAM_TEST_DATA_ODD
	RegSet(0x1F, 0x0000);//SRAM_TEST_DATA_EVEN
	RegSet(0x20, 0x03C7);//BOOST_ROW_EN
	RegSet(0x21, 0x0020);//I_VLN_CONTROL
	RegSet(0x22, 0x0020);//I_VLN_AMP_CONTROL
	RegSet(0x23, 0x0010);//I_VLN_CMP_CONTROL
	RegSet(0x24, 0x001B);//I_OFFSET_CONTROL
	//RegSet(0x25, 0x001A);); //I_BANDGAP_CONTROL - TRIMMED PER DIE
	RegSet(0x26, 0x0004);//I_VLN_VREF_ADC_CONTROL
	RegSet(0x27, 0x000C);//I_VLN_STEP_CONTROL
	RegSet(0x28, 0x0010);//I_VLN_BUF_CONTROL
	RegSet(0x29, 0x0010);//I_MASTER_CONTROL
	RegSet(0x2A, 0x0020);//I_VLN_AMP_60MHZ_CONTROL
	RegSet(0x2B, 0x0003);//VREF_AMP_CONTROL
	RegSet(0x2C, 0x0004);//VREF_ADC_CONTROL
	RegSet(0x2D, 0x0004);//VBOOST_CONTROL
	RegSet(0x2E, 0x0007);//V_HI_CONTROL
	RegSet(0x2F, 0x0003);//V_LO_CONTROL
	RegSet(0x30, 0x0003);//V_AMP_CAS_CONTROL
	RegSet(0x31, 0x001F);//V1_CONTROL_CONTEXTA
	RegSet(0x32, 0x001A);//V2_CONTROL_CONTEXTA
	RegSet(0x33, 0x0012);//V3_CONTROL_CONTEXTA
	RegSet(0x34, 0x0003);//V4_CONTROL_CONTEXTA
	RegSet(0x35, 0x0020);//GLOBAL_GAIN_CONTEXTA_REG
	RegSet(0x36, 0x0010);//GLOBAL_GAIN_CONTEXTB_REG
	RegSet(0x37, 0x0000);//VOLTAGE_CONTROL
	RegSet(0x38, 0x0000);//IDAC_VOLTAGE_MONITOR
	RegSet(0x39, 0x0025);//V1_CONTROL_CONTEXTB
	RegSet(0x3A, 0x0020);//V2_CONTROL_CONTEXTB
	RegSet(0x3B, 0x0003);//V3_CONTROL_CONTEXTB
	RegSet(0x3C, 0x0003);//V4_CONTROL_CONTEXTB
	RegSet(0x46, 0x231D);//DARK_AVG_THRESHOLDS
	RegSet(0x47, 0x0080);//CALIB_CONTROL_REG (AUTO)
	RegSet(0x4C, 0x0002);//STEP_SIZE_AVG_MODE
	RegSet(0x70, 0x0000);//ROW_NOISE_CONTROL
	RegSet(0x71, 0x002A);//NOISE_CONSTANT
	RegSet(0x72, 0x0000);//PIXCLK_CONTROL
	RegSet(0x7F, 0x0000);//TEST_DATA
	RegSet(0x80, 0x04F4);//TILE_X0_Y0
	RegSet(0x81, 0x04F4);//TILE_X1_Y0
	RegSet(0x82, 0x04F4);//TILE_X2_Y0
	RegSet(0x83, 0x04F4);//TILE_X3_Y0
	RegSet(0x84, 0x04F4);//TILE_X4_Y0
	RegSet(0x85, 0x04F4);//TILE_X0_Y1
	RegSet(0x86, 0x04F4);//TILE_X1_Y1
	RegSet(0x87, 0x04F4);//TILE_X2_Y1
	RegSet(0x88, 0x04F4);//TILE_X3_Y1
	RegSet(0x89, 0x04F4);//TILE_X4_Y1
	RegSet(0x8A, 0x04F4);//TILE_X0_Y2
	RegSet(0x8B, 0x04F4);//TILE_X1_Y2
	RegSet(0x8C, 0x04F4);//TILE_X2_Y2
	RegSet(0x8D, 0x04F4);//TILE_X3_Y2
	RegSet(0x8E, 0x04F4);//TILE_X4_Y2
	RegSet(0x8F, 0x04F4);//TILE_X0_Y3
	RegSet(0x90, 0x04F4);//TILE_X1_Y3
	RegSet(0x91, 0x04F4);//TILE_X2_Y3
	RegSet(0x92, 0x04F4);//TILE_X3_Y3
	RegSet(0x93, 0x04F4);//TILE_X4_Y3
	RegSet(0x94, 0x04F4);//TILE_X0_Y4
	RegSet(0x95, 0x04F4);//TILE_X1_Y4
	RegSet(0x96, 0x04F4);//TILE_X2_Y4
	RegSet(0x97, 0x04F4);//TILE_X3_Y4
	RegSet(0x98, 0x04F4);//TILE_X4_Y4
	RegSet(0x99, 0x0000);//X0_SLASH5
	RegSet(0x9A, 0x0096);//X1_SLASH5
	RegSet(0x9B, 0x012C);//X2_SLASH5
	RegSet(0x9C, 0x01C2);//X3_SLASH5
	RegSet(0x9D, 0x0258);//X4_SLASH5
	RegSet(0x9E, 0x02F0);//X5_SLASH5
	RegSet(0x9F, 0x0000);//Y0_SLASH5
	RegSet(0xA0, 0x0060);//Y1_SLASH5
	RegSet(0xA1, 0x00C0);//Y2_SLASH5
	RegSet(0xA2, 0x0120);//Y3_SLASH5
	RegSet(0xA3, 0x0180);//Y4_SLASH5
	RegSet(0xA4, 0x01E0);//Y5_SLASH5
	RegSet(0xA5, 0x003A);//DESIRED_BIN
	RegSet(0xA6, 0x0002);//EXP_SKIP_FRM_H
	RegSet(0xA8, 0x0000);//EXP_LPF
	RegSet(0xA9, 0x0002);//GAIN_SKIP_FRM
	RegSet(0xAA, 0x0002);//GAIN_LPF_H
	RegSet(0xAB, 0x0040);//MAX_GAIN
	RegSet(0xAC, 0x0001);//MIN_COARSE_EXPOSURE
	RegSet(0xAD, 0x01E0);//MAX_COARSE_EXPOSURE
	RegSet(0xAE, 0x0014);//BIN_DIFF_THRESHOLD
	RegSet(0xAF, 0x0000);//AUTO_BLOCK_CONTROL
	RegSet(0xB0, 0xABE0);//PIXEL_COUNT
	RegSet(0xB1, 0x0002);//LVDS_MASTER_CONTROL
	RegSet(0xB2, 0x0010);//LVDS_SHFT_CLK_CONTROL
	RegSet(0xB3, 0x0010);//LVDS_DATA_CONTROL
	RegSet(0xB4, 0x0000);//LVDS_DATA_STREAM_LATENCY
	RegSet(0xB5, 0x0000);//LVDS_INTERNAL_SYNC
	RegSet(0xB6, 0x0000);//LVDS_USE_10BIT_PIXELS
	RegSet(0xB7, 0x0000);//STEREO_ERROR_CONTROL
	RegSet(0xBF, 0x0016);//INTERLACE_FIELD_VBLANK
	RegSet(0xC0, 0x000A);//IMAGE_CAPTURE_NUM
	RegSet(0xC2, 0x18D0);//ANALOG_CONTROLS
	RegSet(0xC3, 0x007F);//AB_PULSE_WIDTH_REG
	RegSet(0xC4, 0x007F);//TX_PULLUP_PULSE_WIDTH_REG
	RegSet(0xC5, 0x007F);//RST_PULLUP_PULSE_WIDTH_REG
	RegSet(0xC6, 0x0000);//NTSC_FV_CONTROL
	RegSet(0xC7, 0x4416);//NTSC_HBLANK
	RegSet(0xC8, 0x4421);//NTSC_VBLANK
	RegSet(0xC9, 0x0002);//COL_WINDOW_START_CONTEXTB_REG
	RegSet(0xCA, 0x0004);//ROW_WINDOW_START_CONTEXTB_REG
	RegSet(0xCB, 0x01E0);//ROW_WINDOW_SIZE_CONTEXTB_REG
	RegSet(0xCC, 0x02EE);//COL_WINDOW_SIZE_CONTEXTB_REG
	RegSet(0xCD, 0x0100);//HORZ_BLANK_CONTEXTB_REG
	RegSet(0xCE, 0x0100);//VERT_BLANK_CONTEXTB_REG
	RegSet(0xCF, 0x0190);//COARSE_SHUTTER_WIDTH_1_CONTEXTB
	RegSet(0xD0, 0x01BD);//COARSE_SHUTTER_WIDTH_2_CONTEXTB
	RegSet(0xD1, 0x0064);//SHUTTER_WIDTH_CONTROL_CONTEXTB
	RegSet(0xD2, 0x01C2);//COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTB
	RegSet(0xD3, 0x0000);//FINE_SHUTTER_WIDTH_1_CONTEXTA
	RegSet(0xD4, 0x0000);//FINE_SHUTTER_WIDTH_2_CONTEXTA
	RegSet(0xD5, 0x0000);//FINE_SHUTTER_WIDTH_TOTAL_CONTEXTA
	RegSet(0xD6, 0x0000);//FINE_SHUTTER_WIDTH_1_CONTEXTB
	RegSet(0xD7, 0x0000);//FINE_SHUTTER_WIDTH_2_CONTEXTB
	RegSet(0xD8, 0x0000);//FINE_SHUTTER_WIDTH_TOTAL_CONTEXTB
	RegSet(0xD9, 0x0000);//MONITOR_MODE_CONTROL

//Set binning
	out_byte_low = (Byte) (config.h_binning) + ((Byte) (config.w_binning) << 2);
	out_byte_low|=1<<4;
	out_byte_low|=1<<5;
	m_sccb.SendByte(0xB8 >> 1, 0x0D, 3);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, out_byte_low);

//Set window
	m_sccb.SendByte(0xB8 >> 1, 0x01, 0);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 1);
	m_sccb.SendByte(0xB8 >> 1, 0x02, 0);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 4);

//Set resolution
	m_sccb.SendByte(0xB8 >> 1, 0x03, 480 >> 8);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 480 & 0b11111111);
	m_sccb.SendByte(0xB8 >> 1, 0x04, 752 >> 8);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 752 & 0b11111111);

	RegSet(0xAC, 0x0001);
	RegSet(0xAD, 0x01E0);
	RegSet(0x2C, 0x0004);

	RegSet(0x0F, 0x0000);
	RegSet(0x0F, 0x0101);// 0x0F bit8:1HDR,0linear; bit1:1color,0gray;bit0:1HDR,0linear
	RegSet(0x07, 0x0188);//Context A
	RegSet(0x70, 0);//0x70  0x0000
	RegSet(0xAF, 0x0302);//0xAF  AEC/AGC A~bit0:1AE;bit1:1AG/B~bit2:1AE;bit3:1AG

	RegSet(0xAC, 0x0001);//0xAC  min fine width   0x0001
	RegSet(0xAD, 0x01E0);//0xAD  max fine width   0x01E0-480
	RegSet(0xAB, 50);//0xAB  max analog gain     64

	RegSet(0xB0, 188*120);
	RegSet(0x1C, 0x0303);//0x1C  here is the way to regulate darkness :)

	RegSet(0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
	RegSet(0x20,0x03C7);//Recommended by design to improve performance in HDR mode and when frame rate is low.
	RegSet(0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
	RegSet(0x2B,0x0003);//Improves column FPN.
	RegSet(0x2F,0x0003);//Improves FPN at near-saturation.

	//100DB
	RegSet(0x08,0x01BB);//0x08 Coarse Shutter IMAGEW 1
	RegSet(0x09,0x01D9);//0x09 Coarse Shutter IMAGEW 2
	RegSet(0x0A,0x0164);//0x0A Coarse Shutter IMAGEW Control
	RegSet(0x32, 0x001A);//0x32   0x001A
	RegSet(0x0B,158);//0x0B Coarse Shutter IMAGEW Total
	RegSet(0x0F,0x0103);//0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
	RegSet(0xA5, 60);//0xA5  image brightness  50  1-64
	RegSet(0x35,0x8010);//0x35
	RegSet(0x06, 0x0002);

//Anti-eclipse enable
	RegSet(0xC2,0x18D0);

//Register Fix
	RegSet(0x20,0x03C7);
	m_sccb.SendByte(0xB8 >> 1, 0x24, 0x001B>>8);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 0x001B&0b11111111);
	m_sccb.SendByte(0xB8 >> 1, 0x2B, 0);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 3);
	m_sccb.SendByte(0xB8 >> 1, 0x2F, 0);
	m_sccb.SendByte(0xB8 >> 1, 0xF0, 3);
	RegSet(0x13,0x2D2E);

	m_front_buf.reset(new Byte[m_buf_size]);
	memset(m_front_buf.get(), 0, m_buf_size);
	m_back_buf.reset(new Byte[m_buf_size]);

	RegSet(0x0C, 0x03);//0x0c  ¸´Î»

	InitDma();

	m_vsync = Gpi(GetVsyncConfig(std::bind(&MT9V034::OnVsync, this, placeholders::_1)));
// Set DMA to a higher priority to prevent VSYNC being processed earlier
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY - 2);
	NVIC_SetPriority(PORTA_IRQn, __BASE_IRQ_PRIORITY - 1);
}

MT9V034::~MT9V034() {
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY);
}

void MT9V034::InitDma() {
	m_data_array.ConfigValueAsDmaSrc(&m_dma_config);
	m_dma_config.dst.addr = m_front_buf.get();
	m_dma_config.dst.offset = 1;
	m_dma_config.dst.size = Dma::Config::TransferSize::k1Byte;
	m_dma_config.dst.major_offset = -m_buf_size;
	m_dma_config.major_count = m_buf_size;
	m_dma_config.complete_isr = std::bind(&MT9V034::OnDmaComplete, this, placeholders::_1);
	m_dma_config.mux_src = EnumAdvance(DmaMux::Source::kPortA, PinUtils::GetPort(LIBSC_MT9V034_PCLK));
	m_dma = DmaManager::New(m_dma_config, LIBSC_MT9V034_DMA_CH);
	m_front_buffer_writing=true;
}

void MT9V034::Start() {
	m_is_available = false;
	m_is_dma_start = false;
	m_is_shoot = true;
}

void MT9V034::Stop() {
	m_is_available = false;
	m_is_shoot = false;
}

const Byte* MT9V034::LockBuffer() {
	m_is_available=false;
	m_is_lock_buffer=true;
	if(m_front_buffer_writing) {
		return m_back_buf.get();
	} else {
		return m_front_buf.get();
	}
}

void MT9V034::UnlockBuffer() {
	m_is_lock_buffer = false;
}

void MT9V034::OnVsync(Gpi*) {
	LOG_VL("MT9V034 vsync");
	if (!m_is_shoot) {
		return;
	}
	if (!m_is_dma_start) {
		m_is_dma_start = true;
		m_dma->Start();
		return;
	}
	if(!m_is_lock_buffer) {
		m_dma_config.dst.addr = m_front_buffer_writing?m_front_buf.get():m_back_buf.get();
		m_dma->Reinit(m_dma_config);
		m_dma->Start();
	}
}

void MT9V034::OnDmaComplete(Dma*) {
	LOG_VL("MT9V034 line complete");
	m_is_available=true;
	m_front_buffer_writing=!m_front_buffer_writing;
}

#else
MT9V034::MT9V034(const Config&) :
		m_sccb(nullptr), m_data_array(nullptr), m_dma(nullptr), m_w(0), m_h(0), m_is_shoot(false), m_is_lock_buffer(false), m_is_available(false), m_is_dma_start(false) {
	LOG_DL("Configured not to use MT9V034");
}
MT9V034::~MT9V034() {
}
void MT9V034::Start() {
}
void MT9V034::Stop() {
}
Byte** MT9V034::LockBuffer() {
	return nullptr;
}
void MT9V034::UnlockBuffer() {
}

#endif /* LIBSC_USE_MT9V034 */

}
}
