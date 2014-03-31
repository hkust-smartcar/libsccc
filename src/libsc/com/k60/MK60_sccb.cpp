/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：
 * 描述         ：
 *
 * 实验平台     ：
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>
#include <log.h>

#include "libsc/com/config.h"
#include "libsc/com/k60/macro.h"
#include "libsc/com/k60/MK60_sccb.h"
#include "libutil/misc.h"

#define DEV_ADR 0x42 /*設備位址定義*/

#define SET_SCL() LIBSC_PIN_OUT(LIBSC_CAMERA_SCL) = 1
#define RESET_SCL() LIBSC_PIN_OUT(LIBSC_CAMERA_SCL) = 0

#define SET_SDA() LIBSC_PIN_OUT(LIBSC_CAMERA_SDA) = 1
#define RESET_SDA() LIBSC_PIN_OUT(LIBSC_CAMERA_SDA) = 0
#define GET_SDA() LIBSC_PIN_IN(LIBSC_CAMERA_SDA)
#define SET_SDA_GPO() LIBSC_PIN_DDR(LIBSC_CAMERA_SDA) = 1
#define SET_SDA_GPI() LIBSC_PIN_DDR(LIBSC_CAMERA_SDA) = 0

#define SCCB_DELAY() DELAY_US(60)

/*******************************************************************************
* Function Name  : SCCB_delay
* Description    : 延遲時間
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/*
static void SCCB_delay(volatile uint16_t i)
{
	while(i)
	{
		i--;
	}
}
*/

/********************************************************************
 * 函數名：SCCB_Configuration
 * 描述  ：SCCB管腳配置
 * 輸入  ：無
 * 輸出  ：無
 * 注意  ：無
 ********************************************************************/
void SCCB_GPIO_init()
{
	gpio_init(LIBSC_CAMERA_SCL, GPO, 0); //初始化SCL
	gpio_init(LIBSC_CAMERA_SDA, GPO, 0); //初始化SDA
}

/********************************************************************
 * 函數名：SCCB_Start
 * 描述  ：SCCB起始信號
 * 輸入  ：無
 * 輸出  ：無
 * 注意  ：內部調用
 ********************************************************************/
static uint8_t SCCB_Start()
{
	SET_SDA();
	SET_SCL();
	SCCB_DELAY();

	SET_SDA_GPI();
	if(!GET_SDA())
	{
		SET_SDA_GPO();
		return 0; /* SDA線為低電平則匯流排忙,退出 */
	}
	SET_SDA_GPO();
	RESET_SDA();

	SCCB_DELAY();

	SET_SDA_GPI();
	if(GET_SDA())
	{
		SET_SDA_GPO();
		return 0; /* SDA線為高電平則匯流排出錯,退出 */
	}
	SET_SDA_GPO();
	RESET_SDA();
	//SCCB_DELAY();
	return 1;
}

/********************************************************************
 * 函數名：SCCB_Stop
 * 描述  ：SCCB停止信號
 * 輸入  ：無
 * 輸出  ：無
 * 注意  ：內部調用
 ********************************************************************/
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

/********************************************************************
 * 函數名：SCCB_Ack
 * 描述  ：SCCB應答方式
 * 輸入  ：無
 * 輸出  ：無
 * 注意  ：內部調用
 ********************************************************************/
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

/********************************************************************
 * 函數名：SCCB_NoAck
 * 描述  ：SCCB 無應答方式
 * 輸入  ：無
 * 輸出  ：無
 * 注意  ：內部調用
 ********************************************************************/
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

/********************************************************************
 * 函數名：SCCB_WaitAck
 * 描述  ：SCCB 等待應答
 * 輸入  ：無
 * 輸出  ：返回為:=1有ACK,=0無ACK
 * 注意  ：內部調用
 ********************************************************************/
static int SCCB_WaitAck()
{
	RESET_SCL();
	SCCB_DELAY();
	SET_SDA();
	SCCB_DELAY();

	SET_SCL();

	SET_SDA_GPI();
	SCCB_DELAY();

	if(GET_SDA())
	{
		SET_SDA_GPO();
		RESET_SCL();
		return 0;
	}
	SET_SDA_GPO();
	RESET_SCL();
	return 1;
}

/*******************************************************************
* 函數名：SCCB_SendByte
* 描述  ：資料從高位到低位元
* 輸入  ：SendByte: 發送的資料
* 輸出  ：無
* 注意  ：內部調用
*********************************************************************/
static void SCCB_SendByte(uint8_t send_byte)
{
	uint8_t i = 8;
	while(i--)
	{
		RESET_SCL();
		SCCB_DELAY();
		if(send_byte & 0x80)
		{
			SET_SDA();
		}
		else
		{
			RESET_SDA();
		}
		send_byte <<= 1;
		SCCB_DELAY();
		SET_SCL();
		SCCB_DELAY();
	}
	RESET_SCL();
}

/******************************************************************
* 函數名：SCCB_ReceiveByte
* 描述  ：資料從高位到低位元
* 輸入  ：無
* 輸出  ：SCCB匯流排返回的資料
* 注意  ：內部調用
*******************************************************************/
static int SCCB_ReceiveByte()
{
	uint8_t i = 8;
	uint8_t ReceiveByte = 0;

	SET_SDA();
	SCCB_DELAY();
	SET_SDA_GPI();

	while(i--)
	{
		ReceiveByte <<= 1;
		RESET_SCL();
		SCCB_DELAY();
		SET_SCL();
		SCCB_DELAY();

		if(GET_SDA())
		{
			ReceiveByte |= 0x01;
		}

	}
	SET_SDA_GPO();
	RESET_SCL();
	return ReceiveByte;
}

static int SCCB_WriteByte_one(uint16_t write_addr, uint8_t send_byte)
{
	if(!SCCB_Start())
	{
		return 0;
	}
	SCCB_SendByte(DEV_ADR); /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
	}
	SCCB_SendByte((uint8_t)(write_addr & 0x00FF)); /* 設置低起始位址 */
	SCCB_WaitAck();
	SCCB_SendByte(send_byte);
	SCCB_WaitAck();
	SCCB_Stop();
	return 1;
}

/*****************************************************************************************
* 函數名：SCCB_WriteByte
* 描述  ：寫一位元組資料
* 輸入  ：- WriteAddress: 待寫入位址 	- SendByte: 待寫入資料	- DeviceAddress: 器件類型
* 輸出  ：返回為:=1成功寫入,=0失敗
* 注意  ：無
*****************************************************************************************/
int SCCB_WriteByte(uint16_t write_addr , uint8_t send_byte)
{
	//考慮到用sccb的管腳模擬，比較容易失敗，因此多試幾次
	uint8_t i= 0;
	while(!SCCB_WriteByte_one(write_addr, send_byte))
	{
		i++;
		if(i == 20)
		{
			return 0 ;
		}
	}
	return 1;
}

static int SCCB_ReadByte_one(uint8_t *buffer, uint16_t length, uint8_t read_addr)
{
	if(!SCCB_Start())
	{
		return 0;
	}
	SCCB_SendByte(DEV_ADR); /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
	}
	SCCB_SendByte(read_addr); /* 設置低起始位址 */
	SCCB_WaitAck();
	SCCB_Stop();

	if(!SCCB_Start())
	{
		return 0;
	}
	SCCB_SendByte(DEV_ADR + 1); /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
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
	return 1;
}

/******************************************************************************************************************
 * 函數名：SCCB_ReadByte
 * 描述  ：讀取一串數據
 * 輸入  ：- pBuffer: 存放讀出資料 	- length: 待讀出長度	- ReadAddress: 待讀出地址		 - DeviceAddress: 器件類型
 * 輸出  ：返回為:=1成功讀入,=0失敗
 * 注意  ：無
 **********************************************************************************************************************/
int SCCB_ReadByte(uint8_t *buffer, uint16_t length, uint8_t read_addr)
{
	uint8_t i= 0;
	while(!SCCB_ReadByte_one(buffer, length, read_addr))
	{
		i++;
		if(i == 20)
		{
			return 0;
		}
	}
	return 1;
}
