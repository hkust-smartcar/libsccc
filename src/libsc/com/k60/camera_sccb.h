/*
 * camera_sccb.h
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
 * @file       FIRE_SCCB.h
 * @brief      OV攝像頭配置匯流排SCCB函式程式庫
 * @author     野火科技
 * @version    v5.0
 * @date       2013-09-01
 */

#ifndef CAMERA_SCCB_H_
#define CAMERA_SCCB_H_

#include <cstdint>

void SCCB_GPIO_init(void);
bool SCCB_WriteByte(uint16_t write_addr, uint8_t send_byte);
bool SCCB_ReadByte(uint8_t *buffer, uint16_t length, uint8_t read_addr);

#endif /* CAMERA_SCCB_H_ */
