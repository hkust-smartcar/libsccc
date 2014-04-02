/*
 * camera_sccb.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技術討論：野火初學論壇 http://www.chuxue123.com
 *
 *     除注明出處外，以下所有內容版權均屬野火科技所有，未經允許，不得用於商業用途，
 *     修改內容時必須保留野火科技的版權聲明。
 *
 * @file       FIRE_SCCB.c
 * @brief      OV攝像頭配置匯流排SCCB函式程式庫
 * @author     野火科技
 * @version    v5.0
 * @date       2013-09-01
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>
#include <log.h>

#include "libsc/com/config.h"
#include "libsc/com/k60/macro.h"
#include "libsc/com/k60/camera_sccb.h"
#include "libutil/misc.h"

#ifdef LIBSC_USE_CAMERA

#define DEV_ADR 0x42 /*設備位址定義*/

#define SET_SCL() LIBSC_PIN_OUT(LIBSC_CAMERA_SCL) = 1
#define RESET_SCL() LIBSC_PIN_OUT(LIBSC_CAMERA_SCL) = 0

#define SET_SDA() LIBSC_PIN_OUT(LIBSC_CAMERA_SDA) = 1
#define RESET_SDA() LIBSC_PIN_OUT(LIBSC_CAMERA_SDA) = 0
#define GET_SDA() LIBSC_PIN_IN(LIBSC_CAMERA_SDA)
#define SET_SDA_GPO() LIBSC_PIN_DDR(LIBSC_CAMERA_SDA) = 1
#define SET_SDA_GPI() LIBSC_PIN_DDR(LIBSC_CAMERA_SDA) = 0

#define SCCB_DELAY() DELAY_US(60)

/*!
 *  @brief      SCCB延遲函數
 *  @param      time    延時時間
 *  @since      v5.0
 */
/*
static void SCCB_delay(volatile uint16_t i)
{
	while(i)
	{
		i--;
	}
}
*/

/*!
 *  @brief      SCCB管腳配置
 *  @since      v5.0
 */
void SCCB_GPIO_init()
{
	gpio_init(LIBSC_CAMERA_SCL, GPO, 1); //初始化SCL
	gpio_init(LIBSC_CAMERA_SDA, GPO, 1); //初始化SDA

    port_init_NoALT(LIBSC_CAMERA_SCL, ODO | PULLUP);
    port_init_NoALT(LIBSC_CAMERA_SDA, ODO | PULLUP);
}

/*!
 *  @brief      SCCB起始信號
 *  @since      v5.0
 */
static bool SCCB_Start()
{
	SET_SDA();
	SET_SCL();
	SCCB_DELAY();

	SET_SDA_GPI();
	if(!GET_SDA())
	{
		SET_SDA_GPO();
		return false; /* SDA線為低電平則匯流排忙,退出 */
	}
	SET_SDA_GPO();
	RESET_SDA();

	SCCB_DELAY();
	RESET_SCL();

	SET_SDA_GPI();
	if(GET_SDA())
	{
		SET_SDA_GPO();
		return false; /* SDA線為高電平則匯流排出錯,退出 */
	}
	SET_SDA_GPO();
	RESET_SDA();
	//SCCB_DELAY();
	return true;
}

/*!
 *  @brief      SCCB停止信號
 *  @since      v5.0
 */
static void SCCB_Stop()
{
	RESET_SCL();
	//SCCB_DELAY();
	RESET_SDA();
	SCCB_DELAY();
	SET_SCL();
	//SCCB_DELAY();
	SET_SDA();
	SCCB_DELAY();
}

/*!
 *  @brief      SCCB應答信號
 *  @since      v5.0
 */
static void SCCB_Ack()
{
	RESET_SCL();
	SCCB_DELAY();
	RESET_SDA();
	SCCB_DELAY();
	SET_SCL();
	SCCB_DELAY();
	RESET_SCL();
	SCCB_DELAY();
}

/*!
 *  @brief      SCCB無應答信號
 *  @since      v5.0
 */
static void SCCB_NoAck()
{
	RESET_SCL();
	SCCB_DELAY();
	SET_SDA();
	SCCB_DELAY();
	SET_SCL();
	SCCB_DELAY();
	RESET_SCL();
	SCCB_DELAY();
}

/*!
 *  @brief      SCCB 等待應答
 *  @return     應答結果（0表示無應答，1表示有應答）
 *  @since      v5.0
 */
static bool SCCB_WaitAck()
{
	RESET_SCL();
	SCCB_DELAY();
	SET_SDA();
	SCCB_DELAY();

	SET_SCL();

	SET_SDA_GPI();
	SCCB_DELAY();

	if (GET_SDA())
	{
		SET_SDA_GPO();
		RESET_SCL();
		return false;
	}
	SET_SDA_GPO();
	RESET_SCL();
	return true;

	/*
	RESET_SCL();
    //SET_SDA();
	SET_SDA_GPI();

	SCCB_DELAY();
	SET_SCL();

	SCCB_DELAY();

	if(GET_SDA())
	{
		SET_SDA_GPO();
		RESET_SCL();
		return false;
	}
	SET_SDA_GPO();
	RESET_SCL();
	return true;
	*/
}

/*!
 *  @brief      SCCB 發送的資料
 *  @param      SendByte    需要發送的資料
 *  @since      v5.0
 */
static void SCCB_SendByte(uint8_t send_byte)
{
	uint8_t i = 8;
	while(i--)
	{
		if(send_byte & 0x80) //SDA 輸出資料
		{
			SET_SDA();
		}
		else
		{
			RESET_SDA();
		}
		send_byte <<= 1;
		SCCB_DELAY();
		SET_SCL(); //SCL 拉高，採集信號
		SCCB_DELAY();
		RESET_SCL(); //SCL 時鐘線拉低
		SCCB_DELAY();
	}
	//RESET_SCL();
}

/*!
 *  @brief      接收SCCB匯流排的資料
 *  @return     接收到的資料
 *  @since      v5.0
 */
static int SCCB_ReceiveByte()
{
	uint8_t i = 8;
	uint8_t receive_byte = 0;

	//SET_SDA();
	//SCCB_DELAY();
	SET_SDA_GPI();

	while(i--)
	{
		receive_byte <<= 1;
		RESET_SCL();
		SCCB_DELAY();
		SET_SCL();
		SCCB_DELAY();

		if(GET_SDA())
		{
			receive_byte |= 0x01;
		}

	}
	SET_SDA_GPO();
	RESET_SCL();
	return receive_byte;
}

static bool SCCB_WriteByte_one(uint16_t write_addr, uint8_t send_byte)
{
	if(!SCCB_Start())
	{
		return false;
	}
	SCCB_SendByte(DEV_ADR); /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return false;
	}
	SCCB_SendByte((uint8_t)(write_addr & 0x00FF)); /* 設置低起始位址 */
	SCCB_WaitAck();
	SCCB_SendByte(send_byte);
	SCCB_WaitAck();
	SCCB_Stop();
	return true;
}

/*****************************************************************************************
* 函數名：SCCB_WriteByte
* 描述  ：寫一位元組資料
* 輸入  ：- WriteAddress: 待寫入位址    - SendByte: 待寫入資料  - DeviceAddress: 器件類型
* 輸出  ：返回為:=1成功寫入,=0失敗
* 注意  ：無
*****************************************************************************************/
bool SCCB_WriteByte(uint16_t write_addr , uint8_t send_byte)
{
	//考慮到用sccb的管腳模擬，比較容易失敗，因此多試幾次
	uint8_t i= 0;
	while(!SCCB_WriteByte_one(write_addr, send_byte))
	{
		i++;
		if(i == 20)
		{
			return false;
		}
	}
	return true;
}

static bool SCCB_ReadByte_one(uint8_t *buffer, uint16_t length,
		uint8_t read_addr)
{
	if(!SCCB_Start())
	{
		return false;
	}
	SCCB_SendByte(DEV_ADR); /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return false;
	}
	SCCB_SendByte(read_addr); /* 設置低起始位址 */
	SCCB_WaitAck();
	SCCB_Stop();

	if(!SCCB_Start())
	{
		return false;
	}
	SCCB_SendByte(DEV_ADR + 1); /* 器件地址 */

	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return false;
	}
	while(length)
	{
		*buffer = SCCB_ReceiveByte();
		if(length == 1)
		{
			SCCB_NoAck();
		}
		else
		{
			SCCB_Ack();
		}
		buffer++;
		length--;
	}
	SCCB_Stop();
	return true;
}

/******************************************************************************************************************
 * 函數名：SCCB_ReadByte
 * 描述  ：讀取一串數據
 * 輸入  ：- pBuffer: 存放讀出資料  - length: 待讀出長度    - ReadAddress: 待讀出地址        - DeviceAddress: 器件類型
 * 輸出  ：返回為:=1成功讀入,=0失敗
 * 注意  ：無
 **********************************************************************************************************************/
bool SCCB_ReadByte(uint8_t *buffer, uint16_t length, uint8_t read_addr)
{
	uint8_t i= 0;
	while(!SCCB_ReadByte_one(buffer, length, read_addr))
	{
		i++;
		if(i == 20)
		{
			return false;
		}
	}
	return true;
}

#endif /* LIBSC_USE_CAMERA */
