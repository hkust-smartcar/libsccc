/*
 * ov7725.cpp
 * OV7725 camera
 *
 * Author: Ben Lai, Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：ov7725.h
 * 描述         ：
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include <mini_common.h>
#include <hw_common.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

#include <log.h>
#include <MK60_dma.h>
#include <vectors.h>

#include "libsc/com/config.h"
#include "libsc/com/ov7725.h"
#include "libsc/com/ov7725_reg.h"
#include "libsc/com/k60/camera_sccb.h"
#include "libsc/com/k60/macro.h"
#include "libsc/com/k60/port_isr_manager.h"
#include "libutil/misc.h"

#define REG_COUNT 49

namespace libsc
{

namespace
{

Ov7725 *g_instance = nullptr;

struct RegisterInfo
{
	uint8_t addr; /*寄存器位址*/
	uint8_t val; /*寄存器值*/
};

RegisterInfo* CreateReg(const uint16_t w, const uint16_t h)
{
	uint16_t _w = libutil::Clamp<uint16_t>(1, w, 640);
	uint16_t _h = libutil::Clamp<uint16_t>(1, h, 480);

	RegisterInfo *reg = new RegisterInfo[REG_COUNT];
	RegisterInfo *it = reg;

	//寄存器，寄存器值次
	*(it++) = {OV7725_COM4, 0xC1};
	*(it++) = {OV7725_CLKRC, 0x00};
	*(it++) = {OV7725_COM2, 0x03};
	*(it++) = {OV7725_COM3, 0xD0};
	// QVGA whenever possible
	*(it++) = {OV7725_COM7, ((uint8_t)((_w <= 320 && _h <= 240) ? 0x40 : 0x00))};
	*(it++) = {OV7725_HSTART, 0x3F};
	*(it++) = {OV7725_HSIZE, 0x50};
	*(it++) = {OV7725_VSTRT, 0x03};
	*(it++) = {OV7725_VSIZE, 0x78};
	*(it++) = {OV7725_HREF, 0x00};
	*(it++) = {OV7725_SCAL0, 0x0A};
	*(it++) = {OV7725_AWB_Ctrl0, 0xE0};
	*(it++) = {OV7725_DSPAuto, 0xff};
	*(it++) = {OV7725_DSP_Ctrl2, 0x0C};
	*(it++) = {OV7725_DSP_Ctrl3, 0x00};
	*(it++) = {OV7725_DSP_Ctrl4, 0x00};

	*(it++) = {OV7725_HOutSize, (uint8_t)(_w >> 2)};
	*(it++) = {OV7725_VOutSize, (uint8_t)(_h >> 1)};
	*(it++) = {OV7725_EXHCH, ((uint8_t)(0x00 | ((_h & 0x1) << 2) | (_w & 0x3)))};

	*(it++) = {OV7725_GAM1, 0x0c};
	*(it++) = {OV7725_GAM2, 0x16};
	*(it++) = {OV7725_GAM3, 0x2a};
	*(it++) = {OV7725_GAM4, 0x4e};
	*(it++) = {OV7725_GAM5, 0x61};
	*(it++) = {OV7725_GAM6, 0x6f};
	*(it++) = {OV7725_GAM7, 0x7b};
	*(it++) = {OV7725_GAM8, 0x86};
	*(it++) = {OV7725_GAM9, 0x8e};
	*(it++) = {OV7725_GAM10, 0x97};
	*(it++) = {OV7725_GAM11, 0xa4};
	*(it++) = {OV7725_GAM12, 0xaf};
	*(it++) = {OV7725_GAM13, 0xc5};
	*(it++) = {OV7725_GAM14, 0xd7};
	*(it++) = {OV7725_GAM15, 0xe8};
	*(it++) = {OV7725_SLOP, 0x20};
	*(it++) = {OV7725_LC_RADI, 0x00};
	*(it++) = {OV7725_LC_COEF, 0x13};
	*(it++) = {OV7725_LC_XC, 0x08};
	*(it++) = {OV7725_LC_COEFB, 0x14};
	*(it++) = {OV7725_LC_COEFR, 0x17};
	*(it++) = {OV7725_LC_CTR, 0x05};
	*(it++) = {OV7725_BDBase, 0x99};
	*(it++) = {OV7725_BDMStep, 0x03};
	*(it++) = {OV7725_SDE, 0x04};
	*(it++) = {OV7725_BRIGHT, 0x00};
	*(it++) = {OV7725_CNST, 0xFF};
	*(it++) = {OV7725_SIGN, 0x06};
	*(it++) = {OV7725_UVADJ0, 0x11};
	*(it++) = {OV7725_UVADJ1, 0x02};

	assert(it - reg <= REG_COUNT);
	return reg;
}

}

Ov7725::Ov7725(const uint16_t w, const uint16_t h)
		: m_w(libutil::Clamp<uint16_t>(1, w, 640)),
		  m_h(libutil::Clamp<uint16_t>(1, h, 480)),
		  m_is_buffer_lock(false),
		  m_front_buffer(nullptr), m_back_buffer(nullptr),
		  m_state(END_SHOOT), m_is_shoot_once(true), m_is_image_ready(false)
{
	m_buffer_size = ceil(m_w * m_h / 8.0);
	m_front_buffer = new Byte[m_buffer_size];
	m_back_buffer = new Byte[m_buffer_size];
	g_instance = this;
}

Ov7725::~Ov7725()
{
	DisableIsr((VECTORn_t)(LIBSC_CAMERA_DMA_CH + DMA0_VECTORn));
	SetIsr((VECTORn_t)(LIBSC_CAMERA_DMA_CH + DMA0_VECTORn), nullptr);
	DMA_IRQ_CLEAN((LIBSC_CAMERA_DMA_CH + DMA_CH0)); //清除通道傳輸中斷標誌位元
	DMA_DIS((LIBSC_CAMERA_DMA_CH + DMA_CH0));

	UnregVsyncHandler();

	g_instance = nullptr;
	delete[] m_front_buffer;
	delete[] m_back_buffer;
}

bool Ov7725::Init()
{
	bool is_init = false;
	for (int i = 0; i < 10000 && !is_init; ++i)
	{
		is_init = InitCameraConfig();
	}
	if (!is_init)
	{
		return false;
	}
	else
	{
		InitPort();
		return true;
	}
}

void Ov7725::ShootOnce()
{
	m_is_shoot_once = true;
	m_state = START_SHOOT;
	m_is_image_ready = false;
	RegVsyncHandler();
}

void Ov7725::ShootContinuously()
{
	m_is_shoot_once = false;
	m_state = START_SHOOT;
	m_is_image_ready = false;
	RegVsyncHandler();
}

void Ov7725::StopShoot()
{
	if (m_state != END_SHOOT)
	{
		// Set shoot_once to stop it after the next shot
		m_is_shoot_once = true;
	}
}

const Byte* Ov7725::GetImage()
{
	Byte *product = new Byte[m_buffer_size];
	memcpy(product, LockBuffer(), m_buffer_size);
	UnlockBuffer();
	return product;
}

const Byte* Ov7725::LockBuffer()
{
	m_is_buffer_lock = true;
	m_is_image_ready = false;
	return const_cast<const Byte*>(m_front_buffer);
}

/************************************************
 * 函數名：Ov7725_reg_Init
 * 描述  ：Sensor 寄存器 初始化
 * 輸入  ：無
 * 輸出  ：返回1成功，返回0失敗
 * 注意  ：無
 ************************************************/
bool Ov7725::InitCameraConfig()
{
	SCCB_GPIO_init();

	//DELAY_MS(50);
	if (!SCCB_WriteByte(OV7725_COM7, 0x80)) /*復位sensor */
	{
		LOG_E("SCCB error while writing data");
		return false;
	}

	DELAY_MS(50);

	uint8_t sensor_id_code = 0;
	if (!SCCB_ReadByte(&sensor_id_code, 1, OV7725_VER)) /* 讀取sensor ID號*/
	{
		LOG_E("Error while reading id");
		return false;
	}
	LOG_D("Get ID succeed, SENSOR ID is 0x%x", sensor_id_code);
	LOG_D("Config Register Number is %d", REG_COUNT);
	if (sensor_id_code == OV7725_ID)
	{
		std::unique_ptr<RegisterInfo[]> reg(CreateReg(m_w, m_h));
		for (int i = 0; i < REG_COUNT; ++i)
		{
			if (!SCCB_WriteByte(reg[i].addr, reg[i].val))
			{
				LOG_E("Error while writing register 0x%x", reg[i].addr);
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	LOG_I("OV7725 Register Config Success!");
	return true;
}

void Ov7725::InitPort()
{
	//DMA通道0初始化，PTA27觸發源(默認上升沿)，源地址為PTB_B0_IN，目的地址為：IMG_BUFF，
	//每次傳輸1Byte
	void *src_addr = &((((Dtype *)(&((GPIOX_BASE(LIBSC_CAMERA_DMA_SRC_ADDR))->PDIR)))
			->B[PTn(LIBSC_CAMERA_DMA_SRC_ADDR) >> 3]));
	dma_portx2buff_init((DMA_CHn)(LIBSC_CAMERA_DMA_CH + DMA_CH0),
			src_addr, (void*)m_back_buffer, LIBSC_CAMERA_PCLK, DMA_BYTE1,
			m_buffer_size, DADDR_KEEPON);

	DMA_DIS((LIBSC_CAMERA_DMA_CH + DMA_CH0));
	//disable_irq(PORTA_IRQn); //關閉PTA的中斷
	DMA_IRQ_CLEAN((LIBSC_CAMERA_DMA_CH + DMA_CH0)); //清除通道傳輸中斷標誌位元
	//DMA_IRQ_EN(LIBSC_CAMERA_DMA_CH);
	SetIsr((VECTORn_t)(LIBSC_CAMERA_DMA_CH + DMA0_VECTORn), DmaHandler);
	EnableIsr((VECTORn_t)(LIBSC_CAMERA_DMA_CH + DMA0_VECTORn));

	port_init(LIBSC_CAMERA_PCLK, ALT1 | DMA_RISING | PULLUP); //PCLK
	LIBSC_PIN_DDR(LIBSC_CAMERA_PCLK) = GPO;

	//場中斷，下拉，下降沿觸發中斷，帶濾波
	port_init(LIBSC_CAMERA_VSYNC, ALT1 | IRQ_RISING | PULLUP | PF);
	// GPI
	LIBSC_PIN_DDR(LIBSC_CAMERA_VSYNC) = GPI;

	UnregVsyncHandler();
}

inline void Ov7725::RegVsyncHandler()
{
	PortIsrManager::GetInstance()->SetIsrHandler(PTX(LIBSC_CAMERA_VSYNC),
			PTn(LIBSC_CAMERA_VSYNC), VsyncHandler);
}

inline void Ov7725::UnregVsyncHandler()
{
	PortIsrManager::GetInstance()->SetIsrHandler(PTX(LIBSC_CAMERA_VSYNC),
			PTn(LIBSC_CAMERA_VSYNC), nullptr);
}

void Ov7725::OnVsync()
{
	//場中斷需要判斷是場結束還是場開始
	if (m_state == START_SHOOT) //需要開始採集圖像
	{
		m_state = SHOOTING; //標記圖像採集中
		//關閉VSYNC的中斷
		UnregVsyncHandler();

		DMA_EN(LIBSC_CAMERA_DMA_CH + DMA_CH0); //使能通道CHn 硬體請求
		DMA_DADDR(LIBSC_CAMERA_DMA_CH + DMA_CH0) = (uint32_t)m_back_buffer; //恢復地址
	}
	else
	{
		LOG_E("VSYNC Error");
		m_state = FAIL_SHOOT; //標記圖像採集失敗
		//關閉VSYNC的中斷
		UnregVsyncHandler();
	}
}

__ISR void Ov7725::VsyncHandler()
{
	if (g_instance)
	{
		g_instance->OnVsync();
	}
}

void Ov7725::OnDma()
{
	if (m_is_shoot_once)
	{
		m_state = END_SHOOT;
		DMA_DIS((LIBSC_CAMERA_DMA_CH + DMA_CH0));
	}
	if (!m_is_buffer_lock)
	{
		// Drop the frame as the buffer is locked
		memcpy((Byte*)m_front_buffer, (const Byte*)m_back_buffer, m_buffer_size);
		m_is_image_ready = true;
	}
	DMA_IRQ_CLEAN((LIBSC_CAMERA_DMA_CH + DMA_CH0)); //清除通道傳輸中斷標誌位元
}

__ISR void Ov7725::DmaHandler()
{
	if (g_instance)
	{
		g_instance->OnDma();
	}
}

}
